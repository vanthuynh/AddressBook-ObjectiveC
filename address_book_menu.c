#pragma warning(disable : 4996)
#include <stdio.h>
// #include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "address_book_fops.h"
//#include "abk_log.h"
#include "address_book_menu.h"
#include "address_book.h"

/* clean the input buffer */
void clean_stdin()
{
	while (getchar() != '\n');
}
int getUserInt(const char* prompt)
{
	int userInput;
	while (1)
	{
		printf("%s", prompt);
		scanf_s("%d", &userInput);
		if (userInput >= 0)  return userInput;
		printf("*Note-Enter valid integer number: ");
		clean_stdin();
	}
}
int getBoundedInt(const char* prompt, int lowBound, int highBound)
{
	int userInput;
	while (1)
	{
		userInput = getUserInt(prompt);
		if (lowBound <= userInput && userInput <= highBound)
			break;
		printf("Please enter a value in the range [%d..%d]\nPlease select an option: ", lowBound, highBound);
		clean_stdin();
	}
	return userInput;
}
int get_option(int type, const char* msg)
{
	if (type == NUM)
	{
		int userInput = getBoundedInt(msg, NONE, MAX_DEFAULT_MENU_CHOICE);
		return userInput;
	}
	else if (type == CHAR)
	{
		do
		{
			clean_stdin();
			char charInput;
			printf("%s", msg);
			scanf_s("%c", &charInput, 1);
			switch (charInput)
			{
			case 'n':case 'N':return 'N';
			case 'y':case 'Y':return 'Y';
			}
			printf("Please select Y or N\n");
			fflush(stdout);
		} while (1);
	}
	return NONE;
}
char getChar(const char* prompt)
{
	char charInput;
	do
	{
		int status;
		clean_stdin();
		printf("%s", prompt);
		status = scanf_s("%c", &charInput, 1);
		if (status == 1)	break;
		printf("Invalid character, try again...\n");
		fflush(stdout);
	} while (1);
	return charInput;
}
Status save_prompt(AddressBook* address_book)
{
	char option;

	do
	{
		main_menu();

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

void output_header()
{
	printf("============================================================================================================\n");
	// printf("%0*d\n", 80, 0);
	printf(": %*s : %*s : %*s : %*s\n", -5, "S. No", -35, "Name", -35, "Phone No", -35, "Email Id");
	printf("============================================================================================================\n");
	fflush(stdout);
}
Status list_contacts(AddressBook* address_book, const char* title, int* index, const char* msg, Modes mode)
{
	int counter = 0;
	char option;
	do
	{
		menu_header(title);
		output_header();
		//print each section spaced out correctly
		for (int i = 0; i < mode; i++)
		{
			printf(": %*d : %*s : %*s : %*s\n", -5, address_book->list[counter].si_no
											, -35, address_book->list[counter].name[0]
											, -35, address_book->list[counter].phone_numbers[0]
											, -35, address_book->list[counter].email_addresses[0]);
			counter++;
		}
		printf("============================================================================================================\n");
		while(1)
		{
			option = getChar(msg);
			switch (option)
			{
				case 'q': case 'Q': break;
				case 'p':
					if (counter - 10 >= 0) {
						counter -= 10;
						break;
					}
				case 'n': case 'N': 
					if (counter == address_book->count) {
						printf("%s", "No more contact to show...\n");
						continue;
					}
					break;
				default: continue;
			}
			break;
		}
	} while (toupper(option) != 'Q' && counter <= address_book->count);
	return e_success;
}

void menu_header(const char* str)
{
	fflush(stdout);

	//system("cls");

	printf("#######  Address Book  #######\n");
	if (*str != '\0')
	{
		printf("#######  %s\n", str);
	}
}

void printList(AddressBook* address_book) {
	printf("PRINTING LIST\n");
	for (int i = 0; i < address_book->count; i++) {
		printf("name: %s, number: %s, email: %s\n", address_book->list[i].name[0],
			address_book->list[i].phone_numbers[0], address_book->list[i].email_addresses[0]);
	}
	printf("\nLIST COMPLETE");
}

void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

Status menu(AddressBook* address_book)
{
	ContactInfo backup;
	Status ret;
	int option;

	do
	{
		main_menu();

		option = get_option(NUM, "");
		if ((address_book->count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found!!. Would you like to add? Use Add Contacts");

			continue;
		}

		switch (option)
		{
		case e_add_contact:
			/* Add your implementation to call add_contacts function here */
			add_contacts(address_book);
			break;
		case e_search_contact:
			search_contact(address_book);
			break;
		case e_edit_contact:
			edit_contact(address_book);
			break;
		case e_delete_contact:
			delete_contact(address_book);
			break;
		case e_list_contacts:
			list_contacts(address_book, "Search Result:\n", 0, "Press [q]-Cancel | [n]-Next Page | [p]-Previous Page: ", WINDOW_SIZE);
			break;
			/* Add your implementation to call list_contacts function here */
		case e_save:
			save_file(address_book);
			break;
		case e_exit:
			break;
		}
	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook* address_book)
{
	/* Add the functionality for adding contacts here */
	int user_opt;
	ContactInfo newPerson;	//Declare temp new contact

	menu_header("Add Contact:\n"); //Display header for "Add Contact"

	//Display options user can add to
	printf("0. Exit\n");
	printf("1. Name       :\n");
	printf("2. Phone No 1 :\n");
	printf("3. Email ID 1 :\n");


	do
	{
		user_opt = getBoundedInt("Please select an option: ", 0, 3);

		switch (user_opt)
		{
		case 0:
			break;	//exit
		case 1:
			printf("Enter the name: ");
			scanf("%s", newPerson.name[0]);
			break;
		case 2:
			printf("Enter Phone Number 1: [Please renter the same option of alternate Phone Number]: ");
			scanf("%s", newPerson.phone_numbers[0]);
			break;
		case 3:
			printf("Enter Email ID 1: [Please renter the same option of alternate Email ID]: ");
			scanf("%s", newPerson.email_addresses[0]);
			break;
		}

		/* Display option menu with new added info */
		menu_header("Add Contact:\n");

		printf("0. Exit\n");
		printf("1. Name       : %s\n", newPerson.name[0]);
		printf("2. Phone No 1 : %s\n", newPerson.phone_numbers[0]);
		printf("3. Email ID 1 : %s\n", newPerson.email_addresses[0]);

	} while (user_opt != 0);
	printf("THIS IS THE CURRENT COUNT: %d", address_book->count);

	newPerson.si_no = address_book->count+1;

	address_book->list[address_book->count] = newPerson;	//update latest contact in list
	address_book->count += 1;	//another contact added, increment address book size
	printList(address_book);
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	// add do loop? for exiting search
	// gcc address_book_fops.c address_book_menu.c main.c -o calc
	int counter = 0;
	char option;

	
	menu_header("Search result:\n\n");
	printf("============================================================================================================\n");
		// printf("%0*d\n", 80, 0);
		// printf(": S. No : Name                            : Phone No                        : Email ID                     :\n");
		printf(": %*s : %*s : %*s : %*s\n", -2, "S. No", -40, "Name", -40, "Phone No", -40, "Email Id");
		printf("============================================================================================================\n");

	switch(field){
		case e_no_opt: 
			break;
		case 1:
		for(int i = 0; i < address_book->count ; i++){
			ContactInfo *personfound = &address_book->list[i];
			for (int name = 0; name < 1; name++){
			if(strcasecmp(personfound->name[name], str) == 0){
				
				// serial no, name, phone, email
				printf(": %*d : %*s : %*s : %*s\n", -5, address_book->list[i].si_no, -40, address_book->list[i].name, -40, address_book->list[i].phone_numbers, -40, address_book->list[i].email_addresses);
				counter++;
				printf("============================================================================================================\n");
				}
			}
		}
		break;
	
		case 2:
		
		for(int i = 0; i < address_book->count ; i++){
			
			ContactInfo *personfound = &address_book->list[i];
			for (int phone = 0; phone < 1; phone++){
			if(strcasecmp(personfound->phone_numbers[phone], str) == 0){
				
				// serial no, name, phone, email
				printf(": %*d : %*s : %*s : %*s\n", -5, address_book->list[i].si_no, -40, address_book->list[i].name, -40, address_book->list[i].phone_numbers, -40, address_book->list[i].email_addresses);
				counter++;
				printf("============================================================================================================\n");
				}
			}
			
		}
		
		break;

		case 3:
		
		for(int i = 0; i < address_book->count  ; i++){
			ContactInfo *personfound = &address_book->list[i];
			for (int email = 0; email < 1; email++){
			if(strcasecmp(personfound->email_addresses[email], str) == 0){
				
				// serial no, name, phone, email
				printf(": %*d : %*s : %*s : %*s\n", -5, address_book->list[i].si_no, -40, address_book->list[i].name, -40, address_book->list[i].phone_numbers, -40, address_book->list[i].email_addresses);
				counter++;
				printf("::===============================================================================================================:\n");
				}
			}
		}
		
		break;

		case 4:
		for(int i = 0; i < address_book->count  ; i++){
			ContactInfo *personfound = &address_book->list[i];
			//for (int serial = 0; serial < 1; serial++){
			if(personfound->si_no == atoi(str)){
				
				// serial no, name, phone, email
				printf(": %*d : %*s : %*s : %*s\n", -5, address_book->list[i].si_no, -40, address_book->list[i].name, -40, address_book->list[i].phone_numbers, -40, address_book->list[i].email_addresses);
				counter++;
				printf("::===============================================================================================================:\n");
				}
			//}
		}
	
		break;

		default:
		break;
	}

	char quit;
	while(toupper(quit) != 'Q'){
		printf("Press: [q] | Cancel:  ");
		quit = getchar();
	}
	
	return e_success;
	}
	



Status search_contact(AddressBook *address_book)
{
	// uses contact info struct to set person_found as a pointer
	ContactInfo *person_found = address_book->list;

	// input from user has length of 32
	char input[32]; 

	// declare user options 
	int user_opt;

	
	

	
	//want to shorten this instead of long print statement	
	char *msg = ":======:==================================:==================================:==================================:\n:" 
				": S.No : Name                             : Phone No                         : Email                            :\n:"
				":======:==================================:==================================:==================================:\n"
				":      :                                  :                                  :                                  :\n:"
				":      :                                  :                                  :                                  :\n:"
				":      :                                  :                                  :                                  :\n:";

	// Displays menu options
	menu_header("Search contact by: \n");

	printf("0. Back\n");
	printf("1. Name\n");
	printf("2. Phone No\n");
	printf("3. Email ID\n");
	printf("4. Serial No\n");

	// do loop allows for user to search until case '0' (e_no_opt) is selected
	do {
		
		// user opt calls getbounded function to set options from 0 - 4, will return invalid response if out of bounds
		user_opt = getBoundedInt("\nPlease select an option: ", 0, 100);

		// switch statement based on bounds
		switch (user_opt) {

		// RETURN TO MAIN MENU
		case e_no_opt: 
			break;
		// FIND NAME
		case 1:
			printf("Please enter the name: ");

			// clears input buffer
			clean_stdin();

			// reads input from user, must not exceed 32 characters
			// use %[^\n]%*c to allow user to use both spaces for first/last name, or just first name
			scanf("%[^\n]%*c", input);
			if (search(input, address_book, address_book->count, 1, msg, e_search) == e_success) {
						return search(input, address_book, address_book->count, 1, msg, e_search);
			} 
			
			// NAME FOUND
			
			break;

		// FIND PHONE NUMBER	
		case 2: 
			printf("Please enter the phone number: ");

			// clears input buffer
			clean_stdin();

			// reads input from user, must not exceed 32 characters
			scanf("%s", input);
			if (search(input, address_book, address_book->count, 2, msg, e_search) == e_success) {
						return search(input, address_book, address_book->count, 2, msg, e_search);
			}
				// PHONE NUMBER FOUND
				
			break;
		// FIND EMAIL	
		case 3: 
			printf("Please enter the email ID: ");

			// clears input buffer
			clean_stdin();

			// reads input from user, must not exceed 32 characters
			scanf("%s", input);
			if (search(input, address_book, address_book->count, 3, msg, e_search) == e_success) {
						return search(input, address_book, address_book->count, 3, msg, e_search);
			}
				// EMAIL FOUND
				
			break;
		// FIND SERIAL NUMBER | refers to the index the contact info is stored at
		case 4: 
			printf("Please enter the serial number: ");

			// clears input buffer
			clean_stdin();

			// reads input from user, must not exceed 32 characters
			scanf("%s", input);
			if (search(input, address_book, address_book->count, 4, msg, e_search) == e_success) {
						return search(input, address_book, address_book->count, 4, msg, e_search);
			}
				// SERIAL NUMBER FOUND
				
			break;
		}
	} while (user_opt != e_exit);
}

Status edit_contact(AddressBook* address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook* address_book)
{
	/* Add the functionality for delete contacts here */
}