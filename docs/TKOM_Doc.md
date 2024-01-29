# TKOM 2023Z - Dokumentacja Wstępna
## Wstęp
Język jest pisany jako połączenie dwóch już stworzonych języków - C++ i Python. Ma na celu wydobycie wygodnych cech z obydwu języków i połączenie je w jeden.
Język cechują następujące założenia dotyczące zmiennych:
* Silne Typowanie - Zmienne mają określony typ i nie mogą być porównywane z innymi typami. Nie można bez konwersji przypisać wartości zmiennej jednego typu do zmiennej innego typu.
* Domyślnie Stałe - Zmienne są domyślnie stałe czyli muszą zostać zadeklarowane z wartością, której później nie możemy zmieniać podczas wykonywania programu. Zmienne dynamiczne możemy zadeklarować przy pomocy słowa kluczowego `mut`.
* Przekazywane przez Referencję  - Zmienne przekazywane do funkcji są przekazywane przez referencję czyli funkcja pracuje na oryginalnych zmiennych i zmienia ich wartości. 
## Typy Danych
* Stałe globalne:
	* definiowane poza przed funkcją main()
* Podstawowe:
	* Integer (`int`)
	* Float (`float`)
	* String (`str`)
	* Boolean (`bool`)

* Złożone:
	* Range (`range`)
	* Struct (`struct`)
	
* Parametryzowane:
	* Variant  (`variant`)
	* Pointer (`@`)
	* List (`list`)

## Komentarze
Komentarze nie mogą być zagnieżdżone 
* Komentarze jednoliniowe pisane są za pomocą znaku `#`:
`# to jest komentarz jednoliniowy`
`int::x = 5; # to tez jest komentarz jednoliniowy`

* Komentarze wieloliniowe są pisane za pomocą sekwencji początkowej `/#` i kończone za pomocą sekwencji `#/`. Wszystko pomiędzy tymi sekwencjami jest traktowane jako komentarz i jest ignorowane przez kompilator. Komentarze wieloliniowe nie mogą być zagnieżdżone wewnątrz innych komentarzy wieloliniowych.:
	```xx
	/# to 
	jest komentarz 
	wieloliniowy 
	#/
	```
	```xx
	int::start = 5;
	/# to też jest komentarz 
	wieloliniowy 
	#/
	int::end = 5;
	```
	
	```xx
	int::start = 5;
	/# to też jest BŁĘDNY komentarz !!!
		/#
			Tak nie wpisujemy komentarzy
		#/
	#/
	int::end = 5;
	```

## Deklaracja Typów
* Podstawowe:
```xx
	int::name = 3;
	float:: name1 = 5.0, name2=3.1;
	str::name1=nae=me2="text";
	bool::cond1=0;
	bool::cond2=true;
	bool::cond3=false;
```
* Złożone
	* Range - jest przedziałem czyli prostą listą zawierającą liczby typu Integer od w jakimś zakresie (włączając krańce zakresu) o zadanym kroku. Range działa jak lista czyli możemy odwoływać się do elementów za pomocą indeksów ale nie wspiera metod które ma lista. Nie może być też mutowalny. Parametr krok jest opcjonalny i domyślnie równy 1.
	`range::nazwa = int..int:int;`
	
		```xx
		range::trio = 1..3;		#da zakres {1, 2, 3}
		range::even = 0..10:2;	#da zakres {0, 2, 3, 4, 6, 8, 10}
		range::odd = 1..6:2; #da zakres {1, 3, 5}
		range::step = 1..5:5; #da zakres {1}
		```
	* Struct - reprezentuje strukturę, która może zawierać różne typy. Możemy odwoływać się do konkretnych pól struktury.
		```xx
		struct::name(
			typ1::field1 = value1;
			typ2::field2 = value2;
			...
		);
		```
		```xx
		struct::car (
			str::brand = "Mazda";
			str::model = "CX5";
			bool::for_sale=true;
			int::capacity = 5;
			int::doors_num = 5;
			int:horse_power = 250;
		);
		```
		Struktura jest domyślnie niemutowalna. Tak zdefiniowana strukturę można uzupełnić wartościami tylko przy deklaracji. Nie można zmieniać wartości pól struktury.
		```
		struct::car(
			str::model;
		);
		```
		Strukturę niemutowalną możemy inicjować różnymi wartościami w następujący sposób:
		```
		struct::car(
			str::model="default";
			int::hp=0;
		);

		car::mycar(model="Mustang", hp=250);
		``` 
  		Aby zdefiniować zmienną mutowalną struktury robimy tak:
  		```
    	mut struct::car(model="Mustang", hp=250);
    	```

* Parametryzowane
	* Variant - typ danych który może przechowywać dane różnego typu w jednym miejscu w pamięci. Posiada ukryty znacznik `tag` który informuje jaki typ danych przechowuje w danym momencie.
	Jest domyślnie mutowalny ponieważ nie ma sensu żeby był stały. Zamiast stałego variantu możemy zdefiniować zwykłą stałą zmienną.
	Posiada metode holding() która zwraca typ jaki aktualnie przechowuje.
		```
		variant::name(
			type1,
			type2,
			 ...
		);
		```
		przykłady:
		```
		variant::storage(
			int, 
			string, 
			float
		);
		storage::box_1 = "text";
		```
		

## Struktura i Cechy języka
* Zmienne i Konwersja typów
Zmienne mają stałe typowanie więc aby przypisać do zmiennej string wartośc zmiennnej int musimy użyć konwersji za pomocą słówka kluczowego `as`.
	```xx
		int::number = 5;
		mut string::text = "";
		text = number as[string];
		println(text)	#outputs "5"
	```
Przypisanie do zmiennej int wartości zmiennej float spowoduje zaokrąglenie w dół do liczby całkowitej.
Przypisanie do zmiennej string wartości dowolnego typu spowoduje przekonwertowanie na napis. dla boolean 1 - >string = true, dla boolean = 0 -> string = false.


* Konstrukcje warunkowe
`if, else`
przykłady:
	```xx
		if warunek[
			...
		]
	```
	
	```xx
		if warunek[
			...
		] else [
			...
		]
	```
*  Pętla
	Jedyną dostępną instrukcją z pętlą jest `while`.
	przykłady:
	```xx
		while warunek[
			...
		]
	```
* Funkcje
	* zmienne lokalne o określonym zasięgu
	* przyjmują argumenty przez referencję 
	* mogą być wywoływane rekursywnie
	```xx
		fun type0::name(type1::argument1, type2::argument2, ...)[
			# code goes here
			return type0;
		]
	```
* Funkcje Standardowe
	* `print()` - wypisuje argument na standardowe wyjście
	* `println()` - wypisuje argument na standardowe wyjście kończąc znakiem nowej linii
	* `input()` - pobiera wartość jako string z standardowego wejścia
	* `int::len()` - zwraca ilość znaków w string lub liczbę elementów w tablicy 
* Główna Funkcja Programu
	Głowna funkcja nazywa się `main` i jest typu void (pustego). Funkcja main nie przyjmuje argumentów i nie zwraca wartości. Kończy się pustym statementem `return;`
	```xx
		main()[
			# tu znajduje się kod programu
			return;
		]
	```
* Dołączanie zewnętrznych plików za pomocą słówka kluczowego `usefile` (funkcjonalność opcjonalna)
	`usefile file_name;`

## Przykłady Kodu
* Variables part 1
```xx
	# variables part 1
	# Execution starts from function 'main'
	fun main()[
		int::number = 5;
		float::fraction = 0.5;
		str::text = "some text	'lalala' \n";
		bool::condition_true = true;
		bool::condition_false = 0;
		range::count = 1..5;
		
		# Variables are static by default - cannot change their value
		# Error: Attempt to modify immutable variable:
		# number = 6;
		
		# This is the correct way
		mut int::mutable_number = 0;
		mutable_number = 5;

		# Cannot change type of variable
		# Error: cannot assign value of type str to variable 'mutable_number' of type int`
		# mutable_number = "number in text";
		
		# Variables can be scoped using blocks
		[
			int::scoped_int = 10;
		]
		# Error variable 'scoped_int' is undefined
		# println(scoped_int);
		return;
	]
```
* Variables part 2
```xx
	fun main()[
		# variables are static by default so we must make them mutable
		mut string::number;
		
		# print function takes multiple arguments seperated by coma and outputs them as strings
		print("enter number", "from 0 to 9:");
		
		# takes input from stdin
		number = input();
		
		# prints empty line 
		println()
		print("Wprowadzona liczba to: ", number);
		
		return;
]
```
* Variables part 3
```xx
	fun main()[
		int::age = 20;
		float::weight = 72.5;
		str::name = "tim";
		str::surname = "Jonsky";
		# string concatenation
		str::full_name = name + " " + surname;
		bool::adult = true;
		# mutable boolean variable
		mut bool::alive = 1;
		println("This is ",  full_name, "he is ", age " years old");
		if alive [
			println("he is alive");
		]else[
			println("he is dead");
		]
		alive = 0;
		println("Now value of 'alive' is:", alive); # prints "Now value of 'alive' is:false"
		return;
	]
```
* Conversion examples
```xx
	fun main()[
		int::number = 123;
		float::decimal = 45.67;
		str::text1 = "789";
		str::text2 = "7.9";
		
		# int to str
		str::number_as_text = number as[str];
		println(number_as_text);  # outputs "123"
		
		# float to str
		str::decimal_as_text = decimal as[str];
		println(decimal_as_text);  # outputs "45.67"

		# str to int
		int::text_as_number = text as[int];
		println(text_as_number);  # outputs 789

		# str to float
		float::text_as_decimal = text as[float];
		println(text_as_decimal);  # outputs 7.9

		# Uwaga: Konwersje mogą nie działać dla wszystkich wartości i typów.
		# Na przykład próba konwersji "hello" na int prawdopodobnie spowoduje błąd.

		return;
]
```
* Arithmetics
```xx
	fun main()[
		int::a = 10;
		int::b = 20;
		float::x = 5.5;
		float::y = 2.5;

		# Addition using +
		mut int::c = a;
		c = c + b;
		println(c);  # outputs 30

		# Subtraction using -=
		mut int::d = a;
		d -= b;
		println(d);  # outputs -10

		# Adding a float to an int
		# The result will be a float because of type promotion
		float::add_mixed = a + x;
		println(add_mixed);  # outputs 15.5

		# Multiplying int by float
		# The result will be a float because of type promotion
		float::mul_mixed = b * x;
		println(mul_mixed);  # outputs 110.0

		# If you want the result to be an int, you can explicitly convert it
		int::mul_mixed_int = (b * x) as[int];
		println(mul_mixed_int);  # outputs 110

		# Increment (increase value by 1 using +=)
		mut int::i = 0;
		i += 1;
		println(i);  # outputs 1

		# Decrement (decrease value by 1 using -=)
		mut int::j = 5;
		j -= 1;
		println(j);  # outputs 4

		return;
]
```
* Mutable and immutable variables
```xx
fun main()[
	# Define a mutable variable a with value 5
	mut int::a = 5;

	# Define an immutable variable b and assign a to it
	int::b = a;

	# Modifying a
	a = 10;

	# Printing the values of a and b
	println(a);  # outputs 10
	println(b);  # outputs 10

	# Trying to modify b directly will result in an error
	# b = 15;  # Error: Attempt to modify immutable variable

	return;
]
```
* Functions
```xx
# Void function
fun foobar()[
	println("Im just printing");
	return;
]


# Function for incrementing by 3
fun str::increment(mut int::value)[
	value += 3;
	return "incremented with success";
]

# Function to calculate the area of a rectangle
fun int::area(int::length, int::width)[
	# Calculate area
	int::result = length * width;

	# Return the result
	return result;
]

# Function to calculate the perimeter of a rectangle
fun int::perimeter(int::length, int::width)[
	# Calculate perimeter
	int::result = 2 * (length + width);

	# Return the result
	return result;
]

fun main()[
	# Define mutable variable for incrementation
	mut int::a = 5;
	str::result = increment(a);
	println(a) # outputs 8

	# passing immutable int to function will generate error
	int::b = 5;
	# str::result = increment(b);
	# Error: Attempt to modify passed immutable variable 'b' inside function  'increment'.

	# Define dimensions of the rectangle
	int::l = 5;
	float::w = 10.0;

	# Call the functions and store the results
	int::rect_area = area(l, w as[int]);
	int::rect_perimeter = perimeter(l, w as[int]);

	# Print the results
	println("Area of the rectangle: ", rect_area);  # outputs 50
	println("Perimeter of the rectangle: ", rect_perimeter);  # outputs 30
	
	# using undefined function
	# int::v = volume(l, w, l);
	# Error: Function 'volume' not defined in the current scope. Ensure it's correctly imported or defined.
	
	return;
]
```
* Recursive Functions
```xx
fun int::factorial(int::n) [
    if n <= 1 [
        return 1;
    ] else [
        return n * factorial(n - 1);
    ]
]
```

* Condition statements
```
# Function to check if a number is prime
fun bool::is_prime(int::number)[
	# 0 and 1 are not prime numbers
	if number <= 1 [
		return false;
	]

	# 2 is the only even prime number
	if number == 2 [
		return true;
	]

	# All other even numbers are not primes
	if number % 2 == 0 [
		return false;
	]

	# Check factors up to the square root of the number
	int::limit = number ^ 0.5 as[int];
	mut int::i = 3;

	while i <= limit [
		# If i is a factor of number, number is not prime
		if number % i == 0 [
			return false;
		]
		i += 2; 
	]

	# If no factors were found, then number is prime
	return true;
]

fun main()[

	int::num = input("type number:");  # take number from stdin
	bool::result = is_prime(num);

	# Print whether the number is prime or not
	if result [
		println(num, " is a prime number.");  # Outputs "29 is a prime number."
	] else if !result[
		println(num, " is not a prime number.");
	] else [
		# this line will never execute
	]

	return;
]
```
* Use statement

filename: square.xx :
```xx
# This is the math_functions file
fun int::square(int::num)[
    return num * num;
]
```
filename: program.xx:
```xx
# This is the main_program file

# Import the function from square.xx
usefile square.xx;

fun main()[
    int::number = 4;
    int::result = square(number);
    println("The square of ", number, " is ", result);  # Outputs "The square of 4 is 16"
    return;
]

```

* Pointers, Variants 
file books.xx:
```xx
float::PRICE = 9.81;

struct::book (
	str::title="default";
	str::author="default";
	mut bool::is_borrowed;
	mut float::price;
);

mut range::even = 0..10:2;  # {0, 2, 4, 6, 8, 10}
mut str[3]::genre_list = ("Fiction", "Non-Fiction", "Fantasy");
```
file program.xx
```xx
usefile "books.xx";

fun main()[
	# Creating a book instance
	book::my_book = (title = "The Great Adventure", author = "John Doe", is_borrowed = false, price = 15.99);



	# Using a variant to store a book's status
	variant::status_data = (
		int,
		str,
		bool
	);
	variant status_data::my_status = "true";
	variant status_data::sb_status = 1;

	# check variant current type that it's storing
	if my_status.holding() == int [
		println("im holding int");
	] else [
		println("sb is holding ", sb_status.holding());
	]
	print(status_data);	# outputs 'false'
	status_data = "unknown state"
	print(status_data);	# outputs 'unknown state'

	# Using a range to get even numbers (as an example)
	int::first_even = even[0];  # gets the first even number from the range from books.xx

	mut int::i = 0;
	mut int::j = 0;
	while i < len(matrix[0])[
		while j < len(matrix)/len(matrix[0])[
			print(matrix[i][j], " ");
		]
		println();
	]

	
	# Displaying the details
	println("Title: ", my_book.title);
	println("Author: ", my_book.author);
	println("Price (via pointer): ", @price_ptr);

	return;
]
```


## Przykłady błędów
* Brak pliku który importujemy
```x
usefile file.xx
>> Error [1:9]: File 'filename.txt' not found.
```
*  Użycie niezdefiniowanej zmiennej
```x
int::result = a + b;
>> Error [1:15]: Variable 'a' is undefined.
```
*  definiowanie zdefiniowanej zmiennej
```x
int::result = 5;
int::result = 2;
>> Error [2:6]: Variable 'result' is already defined.
```
* Użycie niezdefiniowanej funkcji
```x
int::result = foo(5);
>> Error [1:15]: Function 'foo' is undefined.
```
* Brak średnika
```x
int::result = a
int::sum = b;
>> Error [1:15]: Missing semicolon at the end of the line.
```
* Nawiasy
```x
fun int::add(int::x, int::y) [
    return x + y;
>> Error [2:17]: Unmatched '['. Expected ']' before end of function.
```
*  Użycie słówka kluczowego jako nazwy
```x
int::fun = 5;
>> Error [1:6]: Cannot use reserved keyword 'fun' as variable name.
```
*  Złe przypisanie typów
```x
str::name = 12345;
>> Error [1:12]: Type mismatch. Expected 'string' but found 'int'.
```
* Zła operacja na typach
```x
str::text = "hello";
int::result = text + 5;
>> Error [2:18]: Type mismatch in operation. Cannot add 'string' and 'int'.
```
* Pozostałę przykłady komunikatów:
```x
Error [5:20]: Mismatched parentheses, expected ')'.
Error [8:33]: Too many arguments in function call to 'function_name'.
Error [9:35]: Too few arguments in function call to 'function_name'.
Error [10:40]: Return type mismatch in function 'function_name'.
Error [11:45]: Expected 'type1', but got 'type2'.
Error [12:50]: Array index out of bounds.
Error [13:55]: Pointer dereference error.
Error [14:60]: Division by zero.
Error [15:65]: Invalid type casting.
Error [16:70]: Incompatible pointer types.
Error [18:80]: 'break' statement not within loop.
Error [19:85]: 'continue' statement not within loop.
Error [27:125]: Invalid use of 'final' keyword.
Error [32:150]: Invalid initializer.
Error [37:175]: Read-only variable is not assignable.
`Error [x:x]: unexpected token: 'something'; wanted any of: identifier, 'if', 'while', 'return', '{'`
```

## Struktura programu
Kompilator jest podzielony na moduły:
*	Czytnik Znaków (CharReader) -> Char
> Czyta wejście (plik lub strumień) znak po znaku i przekazuje te znaki do Leksera gdy ten zgłosi że potrzebuje kolejnego znaku.
*	Analizator Leksykalny (Lekser) -> Token
> Pobiera znaki od CharReader'a i generuje z nich Tokeny. W momencie gdy przetworzy znak zgłasza CharReader'owi, że potrzebuje kolejnego i wtedy on przetwarza kolejny znak i podaje go Lekserowi. 
*	Anlizator Składniowy (Parser) ->  AST
> Pobiera Tokeny od Lekser'a i tworzy z nich drzewo AST. Pobiera Tokeny jeden po drugim z Lekser'a i podejmuje decyzje dotyczące tworzenia drzewa AST w miarę ich otrzymywania.
*	Analizator Semantyczny (SemanticChecker) -> AST
> Sprawdza drzewo AST pod kątek błędów semantycznych.
*	Interpreter drzewa AST -> Program
>  Wykonuje instrukcje z podanego drzewa AST.
## Testowanie
Poza testami jednostkowymi do możliwie każdej napisanej funkcji, testowane będą również moduły w następujący sposób.
* CharReader - Testy jednostkowe przez podawanie ciągów znaków (plik i strumień).
* Lekser - Testy jednostkowe przez podawanie ciągów znaków (plik i strumień).
* Parser - Testy integracyjne przez podawanie ciągów Tokenów.
* SemanticChecker - Testy integracyjne przez podawanie drzew AST
* Interpreter - Testy integracyjne przez podawanie drzew AST
Cały kompilator będzie testowany przez podawanie na wejście plików z kodem źródłowym lub kodu źródłowego w strumieniu. Dzięki temu będziemy testować połączenie poszczególnych komponentów i czy działają ze sobą poprawnie. 

