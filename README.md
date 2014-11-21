IFJ project 69.
================

Vytvaranie testov:
------------------

```
#!/usr/bin/env bash
export dir="Nazov_vytvorenej_zlozky"
export name="Nazov_testu"
export file=".c subory v ktorych je testovana funkcia"
export src='#include "../src/inc.h"
int main(void) {
    error(1, "WHAT THE FUCK\n");
    return 0;
}'
# zdrojak testovacieho mainu v ktorom sa vola testovana funkcia
export input="vstupne data idu cez subor"
export output="test na vystupne data = ako by mal vypadat vystup"
export retCode=1 # aky by mal byt return code
```

Komentovanie:
-------------

http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html

```
/**
* A test class. A more elaborate class description.
*/
class Test
{
public:
	/**
	* An enum.
	* More detailed enum description.
	*/
	enum TEnum {
		TVal1, /**< enum value TVal1. */
		TVal2, /**< enum value TVal2. */
		TVal3 /**< enum value TVal3. */
	}
	*enumPtr, /**< enum pointer. Details. */
	enumVar; /**< enum variable. Details. */
	/**
	* A constructor.
	* A more elaborate description of the constructor.
	*/
	Test();
	/**
	* A destructor.
	* A more elaborate description of the destructor.
	*/
	~Test();
	/**
	* a normal member taking two arguments and returning an integer value.
	* @param a an integer argument.
	* @param s a constant character pointer.
	* @see Test()
	* @see ~Test()
	* @see testMeToo()
	* @see publicVar()
	* @return The test results
	*/
	int testMe(int a,const char *s);
	/**
	* A pure virtual member.
	* @see testMe()
	* @param c1 the first argument.
	* @param c2 the second argument.
	*/
	virtual void testMeToo(char c1,char c2) = 0;
	/**
	* a public variable.
	* Details.
	*/
	int publicVar;
	/**
	* a function variable.
	* Details.
	*/
	int (*handler)(int a,int b);
};
```

Pouzivanie GIT-u:
-----------------

* Commit:
```
git commit -a -m "Some msg"
```

* Rewrite commit:
```
git commit -a --amend
```
ked prepisete commit tak push bude hlasit chybu treba pouzit prepinac -f (force)

* Push to the server: [toto nepouzivajte]
```
git push origin master
```

* Push to the branch: [pouzite iba toto]
```
git push origin xcibul10
```
napriklad ale miesto xcibul10 napiste svoj branch

* Pull changes from server:
```
git pull origin master
```
dufajte ze nebude 'merge conflict'

* To change branch and pull changes from origin master:
```
git fetch
git checkout [branch-name]
git pull origin master
git push origin [branch-name]
```
[branch-name] -> nazov tvojho branchu

* To change (used localy in your workspace) branch to another branch:
```
git fetch
git checkout [branch-name]
```
