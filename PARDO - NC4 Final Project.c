/* Libraries */
#include<ctype.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* Macro Definitions */
#define CODE_SIZE 9
#define DAY_SIZE 10
#define EMPLOYEE_FILE_SIZE 4
#define LEVEL_ONE_RATE 380.00
#define LEVEL_TWO_RATE 450.00
#define LEVEL_THREE_RATE 550.00
#define MAX 30
#define NAME_SIZE 20
#define TIME_SIZE 6
#define WORK_WEEK_SIZE 5
#define YES "YES"
#define NO "NO"
#define COVERAGE_DATE_SIZE 16

/* Structures */
//typedef struct {
//	char fullName[NAME_SIZE];
//	char employeeCode[CODE_SIZE];
//	int salaryLevel;
//	float salaryRate;
//	char coverageDate[MAX];
//	int workHours;
//}EmployeeThird;

typedef struct {
	char employeeCode[CODE_SIZE];
	char fullName[NAME_SIZE];
	int salaryLevel;
}EmployeeFile;

typedef struct {
	char employeeCode[CODE_SIZE];
	char timeIn [WORK_WEEK_SIZE][TIME_SIZE];
	char timeOut [WORK_WEEK_SIZE][TIME_SIZE];
	bool isHoliday [WORK_WEEK_SIZE];
	char overtimeIn [WORK_WEEK_SIZE][TIME_SIZE];
	char overtimeOut [WORK_WEEK_SIZE][TIME_SIZE];
	char coverageDate [COVERAGE_DATE_SIZE];
}EmployeeTimeLog;

typedef struct {
	int hour;
	int min;
}Time;


//GLOBAL VARIABLES
EmployeeFile employeeFiles[EMPLOYEE_FILE_SIZE] = { 
												   {"A02-0001","Juan Santiago", 1},
												   {"A02-0003","Peter Palabrica", 3}, 
												   {"A02-0004","Anthony Cruz", 1}, 
												   {"A02-0005","Emmie Tabada", 2} 
												   									};
												   									  
char* weekdays[DAY_SIZE] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

/** Functions **/

/* Main Functions */
void print_employee_record (EmployeeFile record);
void generate_report (EmployeeFile record);

/* Computations */
int compute_work_hours();
float compute_hourly_rate();
float compute_regular_income();
float compute_overtime_income();
float compute_gross_income();
float compute_net_income();

/* File Handling */
bool populate_employee_file(char *filename, EmployeeFile* fileContents);
EmployeeTimeLog record_weekly_log(char *employeeCode);
EmployeeFile* find_record(char *filename, char *employeeCode);
bool read_file(char *filename);

int main ()
{
	char employeeCode[CODE_SIZE];
	EmployeeFile* employeeRecord;
	EmployeeTimeLog employee;
	int i;
//	char *token;

	/**************************************************************************************************
	
	*	Populates the employee.txt file with the employee records (code, full name, and salary level).*
	
	*	Execute the write_file function only during first time use. once finished, you may remove it 
		or comment it out once the records have been loaded.								   		  *
	
	***************************************************************************************************/

//	populate_employee_file("employee.txt", employeeFiles);
	if ( read_file("employee.txt") ) {
		
		puts("TESDA Payroll System");
		printf("Enter Employee Code: ");
		gets(employeeCode);
		
		employeeRecord = find_record("employee.txt", employeeCode);
	
		if ( employeeRecord != NULL ) {
			
			system("cls");
			print_employee_record(*employeeRecord);
			employee = record_weekly_log((*employeeRecord).employeeCode);
			
			for(i = 0; i < WORK_WEEK_SIZE; i++) {
				printf("(Main)Employee Code: ");
				puts(employee.employeeCode);
				printf("\n(Main)Time in  %s: ", weekdays[i]);
				puts(employee.timeIn[i]);
				printf("\n(Main)Time out  %s: ", weekdays[i]);
				puts(employee.timeOut[i]);
				printf("\nIs %s a holiday : %d", weekdays[i], employee.isHoliday[i]);
				printf("\n(Main)Overtime in  %s: ", weekdays[i]);
				puts(employee.overtimeIn[i]);
				printf("\n(Main)Overtime out  %s: ", weekdays[i]);
				puts(employee.overtimeOut[i]);
			}	
				printf("\n(Main)Coverage date: ");
				puts(employee.coverageDate);
		} else if ( islower(employeeCode[0]) ) {
			
			puts("\nEnsure that the \'A\' is in the correct case.");
			
		} else {
			
			puts("\nSorry. There was no record or input is invalid.");
			
		}

	} else {
		puts("No instance of \"employee.txt\" found in the current directory.\n\nPlease execute \"populate_employee_file\" function and run the program again.");
	}
	return 0;
}

void generate_report (EmployeeFile record)
{	
	print_employee_record(record);
	
	
	puts("Date Covered: ");
//	puts();
	puts("\nTotal Number of Work Hours: ");
//	puts();
	puts("\nOvertime Hours: ");
//	puts();	
	puts("\nRegular Income: ");
//	puts();
	puts("\nOvertime Income: ");
//	puts();
	puts("\nGross Income: ");
//	puts();
}

void print_employee_record (EmployeeFile record)
{
		printf("*************************************************\n\n");
		
		printf("Employee Name: ");
		puts(record.fullName);

		printf("\nEmployee Code: ");
		puts(record.employeeCode);		

		switch(record.salaryLevel) {
			case 1 :
				printf("\nSalary Level: Level %d\n",record.salaryLevel);
				puts("\nSalary Rate: Php 380.00/day");
				break;
			case 2 :
				printf("\nSalary Level: Level %d\n",record.salaryLevel);
				puts("\nSalary Rate: Php 450.00/day");
				break;
			case 3 :
				printf("\nSalary Level: Level %d\n",record.salaryLevel);
				puts("\nSalary Rate: Php 550.00/day");
				break;				
		}
		
		printf("\n*************************************************\n\n");
}

int compute_work_hours ()
{
	int workHours;
	
	return workHours;
}

float compute_hourly_rate ()
{
	float hourlyRate;
	
	
	return hourlyRate;	
}

float compute_regular_income ()
{
	float regularIncome;
	
	return regularIncome;
}

float compute_overtime_income ()
{
	float overtimeIncome;
	
	return overtimeIncome;
}

float compute_gross_income ()
{
	float grossIncome;
	
	return grossIncome;
}

float computeNetIncome ()
{
	float netIncome;
	
	return netIncome;
}

bool populate_employee_file(char *filename, EmployeeFile* fileContents)
{
	bool isSuccessful = false;
	char isHoliday[3];

	FILE *filePointer = fopen(filename, "w");
	
	if ( NULL != filePointer ) {

		fwrite(fileContents, sizeof(EmployeeFile), 4, filePointer);
		
		isSuccessful = true;
		fclose(filePointer);
	} else {
		printf("\nUnable to open file!");
	}
		
	return isSuccessful;
}

EmployeeTimeLog record_weekly_log(char *employeeCode)
{
//	bool isSuccessful = false;
	int i;
	EmployeeTimeLog log;
	char isHoliday[sizeof(YES)];
	
	strcpy(log.employeeCode, employeeCode);

//	FILE *filePointer = fopen(filename, "w");
	for(i = 0; i < WORK_WEEK_SIZE; i++) {
		printf("Enter the Time-in for %s: ", weekdays[i]);
		gets(log.timeIn[i]);
		printf("Enter the Time-out for %s: ", weekdays[i]);
		gets(log.timeOut[i]);
		
		printf("Is %s a holiday? ", weekdays[i]);
		gets(isHoliday);
		strupr(isHoliday);
		log.isHoliday[i] = strcmp(isHoliday, YES) == 0 ? true : false;
		
		printf("Enter the Overtime-in for %s: ", weekdays[i]);
		gets(log.overtimeIn[i]);
		printf("Enter the Overtime-out for %s: ", weekdays[i]);
		gets(log.overtimeOut[i]);
	}
	
	puts("Enter the coverage date of this payroll:");
	gets(log.coverageDate);
	
	system("cls");
	

//	if ( NULL != filePointer ) {
//
//		fwrite(fileContents, sizeof(EmployeeFile), 4, filePointer);
//		
//		isSuccessful = true;
//		fclose(filePointer);
//	} else {
//		printf("\nUnable to open file!");
//	}
		
	return log;
}

EmployeeFile* find_record(char *filename, char *employeeCode)
{
	EmployeeFile* foundRecord = malloc (sizeof(EmployeeFile));
	EmployeeFile record;
	bool isSuccessful = false;
	
	FILE *filePointer = fopen(filename, "r");
	
	if ( NULL == filePointer ) {
		printf("\nFile is not available!");
	} 
	
		while(fread(&record, sizeof(EmployeeFile), 1, filePointer) && isSuccessful == false) 
		{
			if ( strcmp(record.employeeCode, employeeCode) == 0 ) {
				*foundRecord = record;
				isSuccessful = true;
				fclose(filePointer);		
			}
		}
	
		
		if ( isSuccessful == false ) {
			foundRecord = NULL;
		} 
	
	return foundRecord;
}

bool read_file(char *filename)
{
	bool isSuccessful = false;
	
	FILE *filePointer = fopen(filename, "r");
	
	int i;
	
	if ( NULL != filePointer ) {
//		fread(fileContents, sizeof(EmployeeFile), 4, filePointer);
		isSuccessful = true;
		fclose(filePointer);
	}
	
	return isSuccessful;
}

