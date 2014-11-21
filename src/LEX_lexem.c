#include "inc.h"
#include "LEX_lexem.h"

unsigned int LINE_NUM = 1;

void LEX_string(char **s, int ch, unsigned *poz) {
    unsigned nasobok;
    char *tmp;
    if (((*poz)&(STD_LNGTH-1))==(STD_LNGTH-1)) {
        nasobok = (*poz)>>5;                        // sme retardovany a toto znamena (*poz)/32
        tmp = (char *)realloc(*s,(nasobok+2)<<5);
        if (tmp == NULL) {
            log("Realokaciu velkosti: %d realoc nedokazal spravit\n\
    pozicia %d, char %c", (nasobok+2)<<5, *poz, ch);
            if ((tmp = malloc((nasobok+2)<<5)) == NULL)
                error(ERR_INTERNAL, "Chyba alokacie pamete");
            memcpy(tmp, *s, *poz);
            free(*s);
            *s = tmp;
        }
    }
    (*s)[(*poz)++] = (char)ch;
    (*s)[*poz] = '\0';
}

bool LEX_str(FILE *f,char *s,int ch,unsigned *poz,TEnumLexStr *stav) {
    static unsigned p=0;
    char *kon;
    int cis;

    switch (*stav) {
    case START:
        *poz=0;
        s[*poz]='\0';
        if (ch==EOF) error(ERR_SYN,"Neocakavany koniec suboru.");
        else { 
            if (ch=='\'') *stav=MEDZI;
            else {
                LEX_string(&s,ch,poz);
                *stav=RETAZEC;
            }
        } 
    break;
    case RETAZEC:
        if (ch==EOF) error(ERR_SYN,"Neocakavany koniec suboru.");
        else {
            if (ch=='\'') *stav=MEDZI; 
            else LEX_string(&s,ch,poz);
        } 
    break;
    case MEDZI:
        p=*poz;
        if (ch=='\'') {
            LEX_string(&s,ch,poz);
            *stav=RETAZEC;
        } else {
            if (ch=='#') 
                *stav=CISLO;
            else {
                ungetc(ch,f);
                *stav=START;
                return false;
            }
        }
    break;
    case CISLO:
        if (ch==EOF) error(ERR_SYN,"Neocakavany koniec suboru.");
        else {
            if (ch=='\'') {
                LEX_string(&s,ch,poz);
                cis=(int)strtol(s+p,&kon,10);
                if (*kon=='\'') {
                    if (cis>=1 && cis<=255) {
                        *poz=p;
                        LEX_string(&s,cis,poz);
                        *stav=RETAZEC;
                    } else error(ERR_LEX,"Cislo za znakom # je v zlom intervale.");
                } else error(ERR_LEX,"Neocakavany znak za '#'");
            } else LEX_string(&s,ch,poz);
        }
    }
    return true;  
}

bool LEX_num(int c, char *s, unsigned *poz, TStructNumStat NumStatus, FILE * f) {

    if (isdigit(c)) {                               // Ak je znak cislo  
        if (*poz == 0) {                            // Ak sa jedna o prvu cifru, automaticky sa stava 
                                                    // cela cast cisla inicializovana. Nuly sa ukladat nebudu
            NumStatus->partInit = true;
            if (c == '0')
                return true;
        }
        if (NumStatus->realPart)                              // Realna cast sa inicializuje pri prvom cisle za bodkou
            NumStatus->partInit = true;
        if (NumStatus->expPart) {                             // Ak je cislo za e/E/+/- tak sa inicializuje exponent 
                                                    // pri prvom cisle za tymito znakmi. Nuly na zaciatku sa ignoruju
            NumStatus->partInit = true;
            if (((s[*poz-1] == 'e') || 
                 (s[*poz-1] == '+') || 
                 (s[*poz-1] == '-'))&& 
                 (c == '0'))
                return true;
        }
        LEX_string(&s,c,poz);                        // V kazdom pripade sa cislo pripaja do lexemy. 
        return true;
    }
    else if (c == '.') {                            // Ak je znak bodka
        if ( NumStatus->expPart || NumStatus->realPart )                // Error nastava ak cislo obsahuje e/E alebo dalsiu bodku
            error(ERR_LEX, "Znak '%c' nemoze nasledovat za '%s'\n", c, s);
        else {                                      // Inak
            if (NumStatus->partInit && *poz == 0)             // Ak je cela cast cisla inicializovana a 
                                                    // pozicia znaku je 0 (Nastava ak je cela cast cisla tvorena odignorovanymi nulami)
                LEX_string(&s,'0',poz);              // Nahodi nulu na zaciatok
            NumStatus->realPart = true;                       // Aktivuje realnu cast a nahodi bodku do stringu
            NumStatus->partInit = false;
            LEX_string(&s,c,poz);
            return true;
        }
    }
    else if (c == 'e' || c == 'E') {                // Ak je znak e/E
        if (NumStatus->expPart || 
           (NumStatus->realPart && !NumStatus->partInit))               // Chyba nastava ak lexema obsahuje e/E 
                                                    // alebo ma neinicializovanu realnu cast (kombinacia .E/.e)
            error(ERR_LEX, "Medzi '%s' a '%c' bolo ocakavane cislo\n", s, c);
        else {                                      // Inak
            if (NumStatus->partInit && *poz == 0)             // Kontroluje nuly na zaciatku (napr 0000E), tak ako pri bodke
                LEX_string(&s,'0',poz);
            NumStatus->expPart = true;                        // Aktivuje exponent, nahodi e (lowercase) do stringu
            NumStatus->partInit = false;
            LEX_string(&s,'e',poz);
            return true;
        }
    }   
    else if (c == '+' || c == '-') {                // Ak je znak +/-
        if (NumStatus->partInit){                             // Ak je hociaka cast inicializovana, tak je +/- urcite operator, 
                                                    // nahodi ho spat do stringu a ukonci lexemu
            if (*poz == 0 || (NumStatus->expPart &&
                (s[*poz-1] == 'e' || s[*poz-1] == '+' || s[*poz-1] == '-')))
                                                    // Ak je poz stale 0 tak sa jedna o same nuly -> vrati nulu
                LEX_string(&s,'0',poz);
            ungetc(c, f);
            return false;
        }
        else {                                      // Inak
            if(s[*poz-1] == 'e') {                  // Kontroluje ci je posledny znak e, ak ano tak nenastane chyba
                LEX_string(&s,c,poz);
                return true;
            }
            else                                    // inak je chyba (napr E--)
                error(ERR_LEX, "Neocakavany znak v '%s' '%c'\n", s, c);
        }
    }
    else {                                          // ak je znak nieco ine
        if (!NumStatus->partInit)                             // v pripade neinicializovanej casti bude vzdy error, pretoze urcite ocakava cislo
            error(ERR_LEX, "Za '%s' bola ocakavana cislica!\n", s);
        else if (c == EOF)
            error(ERR_SYN, "Neocakavany koniec suboru");
        else if (((c == '*') ||
                  (c == '/') ||
                  (c == '=') ||
                  (c == '<') ||
                  (c == '>') ||
                  (c == ')'))||
                  (c == ';') ||
                  (isspace(c))) {                   // ak je to nejaky znak ktory moze byt bezprostredne za cislom, tak ho uspesne ukonci
            if (*poz == 0 || (NumStatus->expPart &&
                (s[*poz-1] == 'e' || s[*poz-1] == '+' || s[*poz-1] == '-')))
                                                    // Ak je poz stale 0 tak sa jedna o same nuly -> vrati nulu
                LEX_string(&s,'0',poz);
            ungetc(c, f);
            return false;           
        }
        else                                        // inak je to error
            error(ERR_LEX, "Za '%s' sa vyskytol neocakavany znak %c\n", s, c);
    }
    return false;
    /**
     * bude treba do nejakeho headru nahodit typ bool.
     * bude treba nahradit tie printfy errorov realnymi volaniami chyb, ale to sa da hocikedy
     * Este som to netestoval, ale odkrokoval som si par cisel a vyzeralo to celkom dobre, lepsie robit ma to nenapadlo
     * bool NumStatus->realPart, bool NumStatus->expPart, bool partInit budu mimo funkcie, predavane cez odkaz, funkcia si ich meni podla potreby
     * do headru sa pridala kniznica ctype.h kvoli isspace a isdigit
     */
}

bool LEX_operators(FILE *f, TStructLex *Ret, int z, unsigned *i) {
    switch(Ret->lex[0]) {
    case '<':   //skusam <= a <> inak sa vratim na predosly znak a ukoncim
            if(z=='=') {
                Ret->type = OPERATOR_SMALLEQ;
                LEX_string(&Ret->lex, z, i);
            } else if (z=='>') {
                Ret->type = OPERATOR_NEQUAL;
                LEX_string(&Ret->lex, z, i);
            } else if (z == EOF) error(ERR_SYN, "Neocakavany koniec suboru");
            else if(fseek(f,-1,SEEK_CUR)==-1) error(ERR_INTERNAL, "Nastala chyba pri praci so suborom");
            Ret->type = OPERATOR_SMALLER;
            return false;

        case '>':
            if(z=='=') {
                LEX_string(&Ret->lex, z, i);
                Ret->type = OPERATOR_GREATEQ;
            } else if (z == EOF) error(ERR_SYN, "Neocakavany koniec suboru");
            else if(fseek(f,-1,SEEK_CUR)==-1) error(ERR_INTERNAL, "Nastala chyba pri praci so suborom");
            Ret->type = OPERATOR_GREATER;
            return false;

        case ':':
            if(z=='=') {
                LEX_string(&Ret->lex, z, i);
                Ret->type = OPERATOR_ASSIGN;
            } else if (z == EOF) error(ERR_SYN, "Neocakavany koniec suboru");
            else if(fseek(f,-1,SEEK_CUR)==-1) error(ERR_INTERNAL, "Nastala chyba pri praci so suborom");
            Ret->type = DDOT;
            return false;

        case '+' : Ret->type = OPERATOR_PLUS;
        break;

        case '-' : Ret->type = OPERATOR_MINUS;
        break;

        case '*' : Ret->type = OPERATOR_TIMES;
        break;

        case '/' : Ret->type = OPERATOR_DIV;
        break;

        case '=' : Ret->type = OPERATOR_EQUAL;
        break;

        case ';' : Ret->type = STREDNIK;
        break;

        case ',' : Ret->type = CIARKA;
        break;

        case '.' : Ret->type = BODKA;
        break;

        case '(' : Ret->type = LBRACKET;
        break;

        case ')' : Ret->type = RBRACKET;
        break;

        default:
            error(ERR_LEX, "Neplatny operator '%c'\n", Ret->lex[0]);
    }
    if (z == EOF) {
        if (Ret->type != BODKA)
            error(ERR_LEX, "Neocakavany koniec suboru");
        else
            return false;
    }
    if (fseek(f,-1,SEEK_CUR)==-1) error(ERR_INTERNAL, "Nastala chyba pri praci so suborom");
    return false;
}

int LEX_ident(FILE *f, TStructLex *Ret, int z, unsigned *i) {
    if((z>='a' && z<= 'z') || (z>='A' && z<= 'Z') || z == '_' || (z>='0' && z<='9')) {
        if (z>='A' && z<= 'Z') z = z - ('a' - 'A');
    } else if((z>=40 && z<=47) || (z>=58 && z<=62) || isspace(z) || z == '{') { //oddelovac alebo operator, bodka je 46 - mozno nebude vhod
        if(fseek(f,-1,SEEK_CUR)==-1) error(ERR_INTERNAL, "Nastala chyba pri praci so suborom"); //printf("%s, %d\n", Ret->lex, state);
        return false;
    } else if (z == EOF) error(ERR_SYN, "Neocakavany koniec suboru");
    else error(ERR_LEX, "Neocakavany znak '%c' v identifikatore '%s'", z, Ret->lex);
                            //vrati -1 ak je v slove nepoveoleny znak; ako nepovoleny beriem aj zlozenu zatvorku"}"
    LEX_string(&Ret->lex, z, i);
    return -1;
}

int LEX_base (FILE *f, TStructNumStat NumStatus, TEnumStatus *state, TStructLex *Ret, int z, unsigned *i) {
    if((z>='a' && z<= 'z') || (z>='A' && z<= 'Z') || z == '_') {
        if (z>='A' && z<= 'Z') z = z + ('a' - 'A');
        Ret->type = IDENTIFICATOR;
        *state = SLOVO;
    } else {
        if(z>='0' && z<='9') {
            *state = TERM_NUM;
            LEX_num(z, Ret->lex, i, NumStatus, f);
            return -1;
        } else {
            if(z=='\'') {
                *state = TERM_STR;
                Ret->type = STRING_CONST;
                return -1;
            } else if((z<=47 && z>= 40) || (z>=58 && z<=62)) {
                *state = OPER; //+,-,*,/,(), ciarka,bodka,<,>,=,:, strrrednik '+'
            } else if (z == '{') {
                *state = COMMENT;
                return -1;
            } else if (isspace(z)) {
                *state = SPACE;
                return -1;
            } else if (z == EOF) {
                return 1;
            } else error(ERR_LEX, "Neocakavany znak '%c'", z);
        }
    }
    LEX_string(&Ret->lex, z, i);
    return false;
}

void LEX_getLexem(TStructLex *Ret, FILE* f) {
    unsigned i = 0;
    int z = 0, base_ret, iden_ret;
    SNumStat _NumStatus = { 
        .realPart = false,
        .expPart  = false,
        .partInit = true
    };
    TStructNumStat NumStatus = &_NumStatus;
    TEnumStatus state = BASE;
    TEnumLexStr stav = START;

    if (!(Ret->lex = malloc(STD_LNGTH)))
        error(ERR_INTERNAL, "malloc vratil NULL");

    while((z = fgetc(f)) || true) {
        switch (state) {
            case BASE:
                if (z == '\n') LINE_NUM++; 
                base_ret = LEX_base(f, NumStatus, &state, Ret, z, &i);
                if (base_ret == 1) {
                    Ret->type = TYPE_EOF;
                    return;
                } else if (base_ret == 0) {
                    break;
                }
            break;

            case SLOVO: //!< IDENTIFIKATOR
                iden_ret = LEX_ident(f, Ret, z, &i);
                if (iden_ret == false) {
                    return;
                }else if (iden_ret == true) {
                    error(ERR_SYN, "Neocakavany koniec suboru");
                }
            break;

            case TERM_NUM: //cisla, Lukas
                if (!LEX_num(z, Ret->lex, &i, NumStatus, f)) {
                    if (state == TERM_NUM) {
                        if (NumStatus->realPart || NumStatus->expPart) Ret->type = REAL_CONST;
                        else Ret->type = INT_CONST;
                    }
                    if (z == EOF) {
                        error(ERR_SYN, "Neocakavany koniec suboru");
                    }
                    return;
                }
            break;

            case TERM_STR:
                if (!LEX_str(f,Ret->lex,z,&i,&stav)) {
                    Ret->type=STRING_CONST;
                    if (z == EOF) {
                        error(ERR_SYN, "Neocakavany koniec suboru");
                    }
                    return;
                }
            break;

            case OPER:
                LEX_operators(f, Ret, z, &i);
                return;

            case COMMENT :
                if (z == '}') {
                    state = BASE;
                } else if (z == EOF) 
                    error(ERR_SYN, "Neocakavany koniec suboru");
            break;

            case SPACE:
                if (z == EOF) {
                    Ret->type = TYPE_EOF;
                    return;
                } if (!isspace(z)) {
                    state = BASE;
                    if(fseek(f,-1,SEEK_CUR)==-1) error(ERR_INTERNAL, "Nastala chyba pri praci so suborom");
                }
            break;
        }
    }
}

int isKeyWord(PTStructLex lex) {
    for (char ** key = KEY_WORDS; *key != NULL; key++)
        if (strcmp(lex->lex, *key) == 0)
            return (int)(lex->type = (TEnumLexem)(key-KEY_WORDS));
    return -1;
}
