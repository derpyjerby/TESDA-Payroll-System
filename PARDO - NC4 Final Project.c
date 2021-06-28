/* Libraries */
#include<ctype.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* Macro Definitions */
#define ALLOWANCE 500
#define CODE_SIZE 9
#define COVERAGE_DATE_SIZE 16
#define DAY_SIZE 10
#define EMPLOYEE_FILE_SIZE 4
#define HOLIDAY_INCREASE 1.1
#define LEVEL_ONE_HOURLY 47.5
#define LEVEL_ONE_GSIS 0.01
#define LEVEL_TWO_HOURLY 56.25
#define LEVEL_TWO_GSIS 0.015
#define LEVEL_THREE_HOURLY 68.75
#define LEVEL_THREE_GSIS 0.02
#define LUNCH_BREAK_TIME 12
#define MAX_OVT_WORK_HOURS 3
#define MAX_REG_WORK_HOURS 8
#define NAME_SIZE 20
#define NO "NO"
#define OVERTIME_END 20
#define OVERTIME_INCREASE 1.1
#define OVERTIME_MINS 30
#define OVERTIME_START 17
#define REGULAR_END 17
#define REGULAR_START 8
#define TAX_DEDUCTION 0.1
#define TIME_SIZE 6
#define WORK_WEEK_SIZE 5
#define YES "YES"

/* Structures */
typedef struct {
	char employeeCode[CODE_SIZE];
	char fullName[NAME_SIZE];
	int salaryLevel;
}EmployeeFile;

typedef struct {
	EmployeeFile credentials;
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

typedef struct {
	float hoursWorked;
	float income;
}incomeReport;

typedef struct {
	Time timeIn[WORK_WEEK_SIZE];
	Time timeOut[WORK_WEEK_SIZE];
	Time overtimeIn[WORK_WEEK_SIZE];
	Time overtimeOut[WORK_WEEK_SIZE];
}TimeLogs;

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
void print_employee_credentials (EmployeeFile record);
void generate_full_report (EmployeeTimeLog record, incomeReport regular, incomeReport overtime);

/* Computations */
TimeLogs generate_work_hours(EmployeeTimeLog timeLog);
incomeReport compute_regular_income(TimeLogs timeLogs, int salaryLevel, bool isHoliday[]);
incomeReport compute_overtime_income(TimeLogs timeLogs, int salaryLevel, bool isHoliday[]);

/* File Handling */
bool populate_employee_file(char *filename, EmployeeFile* fileContents);
EmployeeTimeLog record_weekly_log(EmployeeFile employee);
EmployeeFile* find_record(char *filename, char *employeeCode);
bool read_file(char *filename);

int main ()
{
	char employeeCode[CODE_SIZE];
	EmployeeFile* employeeRecord;
	EmployeeTimeLog employee;
	TimeLogs employeeTimeStamps;
	incomeReport regularIncomeReport, overtimeIncomeReport;
	/**************************************************************************************************
	
	*	Populates the employee.txt file with the employee records (code, full name, and salary level).*
	
	*	Execute the populate_employee_file function only during first time use or if you do not have the 
		employee.txt file in the directory of your project. once finished, you may remove it or comment 
		it out once the records have been loaded.								   		  			  *
	
	***************************************************************************************************/
	
	populate_employee_file("employee.txt", employeeFiles);
	if ( read_file("employee.txt") ) {
		
		puts("TESDA Payroll System");
		printf("Enter Employee Code: ");
		gets(employeeCode);
		
		employeeRecord = find_record("employee.txt", employeeCode);
		
		if ( employeeRecord != NULL ) {
			
			system("cls");
			print_employee_credentials(*employeeRecord);
			
			employee = record_weekly_log((*employeeRecord));
			
			employeeTimeStamps = generate_work_hours(employee);
			
			regularIncomeReport = compute_regular_income(employeeTimeStamps, employee.credentials.salaryLevel, employee.isHoliday);
			
			overtimeIncomeReport = compute_overtime_income(employeeTimeStamps, employee.credentials.salaryLevel, employee.isHoliday);
			
			generate_full_report(employee, regularIncomeReport, overtimeIncomeReport);

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

void generate_full_report (EmployeeTimeLog record, incomeReport regular, incomeReport overtime)
{
	float grossIncome;
	float netIncome;
	float tax;
	float sss;
	
	grossIncome = regular.income + overtime.income;
	tax = TAX_DEDUCTION * grossIncome;
	
	switch (record.credentials.salaryLevel) {
		case 1 :
			sss = LEVEL_ONE_GSIS * grossIncome;
			break;
		case 2 :
			sss = LEVEL_TWO_GSIS * grossIncome;
			break;
		case 3 :
			sss = LEVEL_THREE_GSIS * grossIncome;
			break;		
	}
	
	netIncome = grossIncome - (tax + sss) + ALLOWANCE;
	
	system("cls");
	print_employee_credentials(record.credentials);
	
	printf("Date Covered: ");
	puts(record.coverageDate);
	printf("\nRegular Work Hours: %.2f\n", regular.hoursWorked);
	printf("\nOvertime Hours: %.2f\n", overtime.hoursWorked);
	printf("\nRegular Income: Php %.2f\n", regular.income);
	printf("\nOvertime Income: Php %.2f\n", overtime.income);
	printf("\nGross Income: %.2f\n", grossIncome);
	puts("\nDeductions:");
	printf("\nTax: %.2f\n", tax);
	printf("\nSSS: %.2f", sss);
	printf("\n\nNet Income (inclusive of Php %d allowance): %.2f\n", ALLOWANCE, netIncome);
	puts("\n*************************************************\n\n");
}

void print_employee_credentials (EmployeeFile record)
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

TimeLogs generate_work_hours (EmployeeTimeLog timeLog)
{
	TimeLogs workHours;
	int i;
	char delimiter[] = ":";
	char *timeToken;

	for(i = 0; i < WORK_WEEK_SIZE; i++){
		// Converting Time In of user into integer
		timeToken = strtok(timeLog.timeIn[i], delimiter);		
		workHours.timeIn[i].hour = atoi(timeToken);
		timeToken = strtok(NULL, delimiter);
		workHours.timeIn[i].min = atoi(timeToken);
		
		// Converting Time Out of user into integer 
		timeToken = strtok(timeLog.timeOut[i], delimiter);
		workHours.timeOut[i].hour = atoi(timeToken);
		timeToken = strtok(NULL, delimiter);
		workHours.timeOut[i].min = atoi(timeToken);
		
		// Converting Overtime In of user into integer
		timeToken = strtok(timeLog.overtimeIn[i], delimiter);
		workHours.overtimeIn[i].hour = atoi(timeToken);	
		timeToken = strtok(NULL, delimiter);
		workHours.overtimeIn[i].min = atoi(timeToken);
		
		// Converting Overtime Out of user into integer
		timeToken = strtok(timeLog.overtimeOut[i], delimiter);
		workHours.overtimeOut[i].hour = atoi(timeToken);
		timeToken = strtok(NULL, delimiter);
		workHours.overtimeOut[i].min = atoi(timeToken);
	}
	
	return workHours;
}


incomeReport compute_regular_income(TimeLogs timeLogs, int salaryLevel, bool isHoliday[])
{
	incomeReport regularIncomeReport;
	float totalHoursWorked = 0;
	float hoursWorked[WORK_WEEK_SIZE] = {MAX_REG_WORK_HOURS, MAX_REG_WORK_HOURS, MAX_REG_WORK_HOURS, MAX_REG_WORK_HOURS, MAX_REG_WORK_HOURS};
	float late = 0;
	float undertime = 0;
	int i;

	
	for(i = 0; i < WORK_WEEK_SIZE; i++){
		// First checks if the employee entered that day or if the day was a holiday.
		if (timeLogs.timeIn[i].hour == 0 && timeLogs.timeIn[i].min == 0 || isHoliday[i]) {
			hoursWorked[i] = 0;
			continue;
		}

		// Computes for late by subtracting each part of the employee's time in with the set normal time in.
		if ( timeLogs.timeIn[i].hour >= REGULAR_START && timeLogs.timeIn[i].min >= 0 ) { 
			if ( timeLogs.timeIn[i].hour > LUNCH_BREAK_TIME ) { // Instance where employee enters after lunch
				late = timeLogs.timeIn[i].hour - REGULAR_START - 1; // Disregards lunch break hour
			} else {	
				late = timeLogs.timeIn[i].hour - REGULAR_START;
			}
			late += timeLogs.timeIn[i].min / 60.00;
		}  
		if ( timeLogs.timeOut[i].hour < REGULAR_END ) { // Checks hour before since 17 is normal timeout.
			if ( timeLogs.timeOut[i].min == 0 ) { // Checks for undertime wihtout extra minutes
				undertime += REGULAR_END - timeLogs.timeOut[i].hour;
			} else { // Checks for undertime with minutes.
				undertime += REGULAR_END - timeLogs.timeOut[i].hour - 1;
			}
				timeLogs.timeOut[i].min = 60 - timeLogs.timeOut[i].min;
				undertime += timeLogs.timeOut[i].min / 60.00;
		}
		
		hoursWorked[i] -= (undertime + late);
		totalHoursWorked += hoursWorked[i];
		undertime = late = 0;
	}

		
		switch(salaryLevel) {
			case 1 :
				regularIncomeReport.income =  totalHoursWorked * LEVEL_ONE_HOURLY;
				break;
			case 2 :
				regularIncomeReport.income =  totalHoursWorked * LEVEL_TWO_HOURLY;
				break;
			case 3 :
				regularIncomeReport.income =  totalHoursWorked * LEVEL_THREE_HOURLY;
				break;				
		}
		
		regularIncomeReport.hoursWorked = totalHoursWorked;
		
	return regularIncomeReport;
}

incomeReport compute_overtime_income(TimeLogs timeLogs, int salaryLevel, bool isHoliday[])
{
	incomeReport overtimeIncomeReport;
	float totalHoursWorked = 0;
	float undertime, late;
	float hoursWorked[WORK_WEEK_SIZE] = {MAX_OVT_WORK_HOURS, MAX_OVT_WORK_HOURS, MAX_OVT_WORK_HOURS, MAX_OVT_WORK_HOURS, MAX_OVT_WORK_HOURS};
//	float hoursWorked[WORK_WEEK_SIZE] = {0};
	int i;
	
	for(i = 0; i < WORK_WEEK_SIZE; i++){
		// First checks if the day was a holiday.
		if (isHoliday[i]) {
			hoursWorked[i] = MAX_REG_WORK_HOURS;
			// Checks if the employee entered despite holiday
			if ( timeLogs.timeIn[i].hour == 0 && timeLogs.timeIn[i].min == 0 ) {
				hoursWorked[i] = 0;
				continue;
			}
			// Computes for late by subtracting each part of the employee's time in with the set normal time in.
			if ( timeLogs.timeIn[i].hour >= REGULAR_START && timeLogs.timeIn[i].min >= 0 ) { 
				if ( timeLogs.timeIn[i].hour > LUNCH_BREAK_TIME ) { // Instance where employee enters after lunch
					late = timeLogs.timeIn[i].hour - REGULAR_START - 1; // Disregards lunch break hour
				} else {	
					late = timeLogs.timeIn[i].hour - REGULAR_START;
				}
				late += timeLogs.timeIn[i].min / 60.00;
			}  
			// Checks hour before since 17 is normal timeout.	
			if ( timeLogs.timeOut[i].hour < REGULAR_END ) { 
				// Checks for undertime without extra minutes
				if ( timeLogs.timeOut[i].min == 0 ) { 
					undertime += REGULAR_END - timeLogs.timeOut[i].hour;
				} else { // Checks for undertime with minutes.
					undertime += REGULAR_END - timeLogs.timeOut[i].hour - 1;
					timeLogs.timeOut[i].min = 60.00 - timeLogs.timeOut[i].min;
				}
					undertime += timeLogs.timeOut[i].min / 60.00;
			}
			hoursWorked[i] -= undertime + late ;
			undertime = late = 0; 
		} else {
			// First checks if the employee worked overtime that day.
			if ( timeLogs.overtimeIn[i].hour == 0 && timeLogs.overtimeIn[i].min == 0 ) {
				hoursWorked[i] = 0;
				continue;
			}
			
			// Computes for late by subtracting each part of the employee's overtime in with the set normal overtime in.
			
			// Checks the minutes first and converts it to hourly percentage.
			if ( timeLogs.overtimeIn[i].min >= OVERTIME_MINS && timeLogs.overtimeIn[i].min < 60) {
				
				timeLogs.overtimeIn[i].min -= OVERTIME_MINS; 
				timeLogs.overtimeIn[i].hour -= OVERTIME_START;

			} else { // 0-29 mins
				
				timeLogs.overtimeIn[i].min += OVERTIME_MINS;
				
				if ( timeLogs.overtimeIn[i].hour > OVERTIME_START ) {;
					timeLogs.overtimeIn[i].hour -= OVERTIME_START + 1;
					printf("18-20 hour = %d\n", timeLogs.overtimeIn[i].hour);
				} else {
					timeLogs.overtimeIn[i].hour -= OVERTIME_START;
				}
			}
				late += timeLogs.overtimeIn[i].min / 60.00;
				late += timeLogs.overtimeIn[i].hour; 
				
			// Checks undertime based by subtracting each part of the overtime out with the set normal overtime out limit
			
			
			// Hours
			if ( timeLogs.overtimeOut[i].hour > OVERTIME_START ) {
				if ( timeLogs.overtimeOut[i].min <= OVERTIME_MINS && timeLogs.overtimeOut[i].min >= 0 ) {
					timeLogs.overtimeOut[i].hour = OVERTIME_END - timeLogs.overtimeOut[i].hour;
				} else {
					timeLogs.overtimeOut[i].hour = OVERTIME_END - timeLogs.overtimeOut[i].hour - 1;
				}
			} else {
					timeLogs.overtimeOut[i].hour = 0;
			}
				undertime += timeLogs.overtimeOut[i].hour;
			// Checks the minutes first and converts it to the hourly percentage
			if ( timeLogs.overtimeOut[i].min > OVERTIME_MINS && timeLogs.overtimeOut[i].min < 60) {
				timeLogs.overtimeOut[i].min = 60 - timeLogs.overtimeOut[i].min;
				timeLogs.overtimeOut[i].min += OVERTIME_MINS;
			} else {
				timeLogs.overtimeOut[i].min = OVERTIME_MINS - timeLogs.overtimeOut[i].min;
			}
			undertime += timeLogs.overtimeOut[i].min / 60.00;
			hoursWorked[i] -=  undertime + late ;
			undertime = late = 0;
		}
			totalHoursWorked += hoursWorked[i];
	}
		switch(salaryLevel) {
			case 1 :
				overtimeIncomeReport.income = totalHoursWorked * LEVEL_ONE_HOURLY * OVERTIME_INCREASE;
				break;
			case 2 :
				overtimeIncomeReport.income = totalHoursWorked * LEVEL_TWO_HOURLY * OVERTIME_INCREASE;
				break;
			case 3 :
				overtimeIncomeReport.income = totalHoursWorked * LEVEL_THREE_HOURLY * OVERTIME_INCREASE;
				break;				
		}

		overtimeIncomeReport.hoursWorked = totalHoursWorked;
	return overtimeIncomeReport;
}

bool populate_employee_file(char *filename, EmployeeFile* fileContents)
{
	bool isSuccessful = false;
	char isHoliday[sizeof(YES)];

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

EmployeeTimeLog record_weekly_log(EmployeeFile employee)
{
	bool isSuccessful = false;
	int i;
	EmployeeTimeLog log, temp;
	char isHoliday[sizeof(YES)];
	
	//Transfers the employee's code, full name, and salary level to the TimeLog being passed.
	log.credentials = employee;
	//Input for the Timelog is entered by user.
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
	fflush(stdin);
	puts("Enter the coverage date of this payroll:");
	gets(log.coverageDate);

	FILE *filePointer = fopen("dtr.txt", "r+");
	
	if ( NULL != filePointer ) { // If file exists.
		i = 0;
		while(fread(&temp, sizeof(EmployeeTimeLog), 1, filePointer) && isSuccessful == false) 
		{
			// Compares employee code with the current record's code being read in the file.
			if ( strcmp(temp.credentials.employeeCode, employee.employeeCode) == 0 ) {
				isSuccessful = true;
				fseek(filePointer, sizeof(EmployeeTimeLog) * i, SEEK_SET);
				fwrite(&log, sizeof(EmployeeTimeLog), 1, filePointer);
				fclose(filePointer);		
			}
			i++;
		}
		
		if ( isSuccessful == false ) {
			filePointer = fopen("dtr.txt", "a");
			puts("Appending to file new record.");
			fwrite(&log, sizeof(EmployeeTimeLog), 1, filePointer);
		}
		
		fclose(filePointer);
	} else { // If it does not.
		filePointer = fopen("dtr.txt", "w");
		fwrite(&log, sizeof(EmployeeTimeLog), 1, filePointer);
		puts("No file found. Making new instance of \"dtr.txt\" on current directory.");
	}
		
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
		// Finds the employee record with the passed employee code.
		while(fread(&record, sizeof(EmployeeFile), 1, filePointer) && isSuccessful == false) 
		{
			// Compares employee code with the current record's code being read in the file.
			if ( strcmp(record.employeeCode, employeeCode) == 0 ) {
				*foundRecord = record; //foundRecord holds the record's address.
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

