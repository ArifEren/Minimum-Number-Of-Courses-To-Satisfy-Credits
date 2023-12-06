/*
written by Arif Eren DUMANLI
*/

#include <stdio.h>
#include <stdlib.h>

void inputCreditCourseCounts(int *credit_count, int *course_count);
void inputCourseCreditsList(int course_credits[], int course_count);
void swapVariableValues(int *a, int *b);
void heapifyModified(int arr[], int arr_indexes[], int N, int i);
void heapSortModified(int arr[], int arr_indexes[], int N);
int findOptimal(int course_credits_sorted[], int course_credits_sorted_indexes[], int courses_taken_count[], int course_count, int credit_count, int course_count_fixed);
void outputResults(int course_credits[], int courses_taken_count[], int course_count, int credit_count);

int main()
{
	//variable declarations
	int i, credit_count, course_count;
	
	//getting credit count and course count from the user
	inputCreditCourseCounts(&credit_count, &course_count);
	
	//list declarations
	int course_credits[course_count],
	course_credits_sorted[course_count],
	course_credits_sorted_indexes[course_count],
	courses_taken_count[course_count];
	
	//getting course credits for each course from the user
	inputCourseCreditsList(course_credits, course_count);
	
	//firstly copying course credits list, then sorting the 
	//copy of list of course credits in increasing order while 
	//saving indexes to later use for mapping
	for(i = 0; i < course_count; i++)
	{
		course_credits_sorted[i] = course_credits[i];
		course_credits_sorted_indexes[i] = i;
	}
	heapSortModified(course_credits_sorted, course_credits_sorted_indexes, course_count);
	
	//filling a list with zeros to later store results
	for(i = 0; i < course_count; i++)
		courses_taken_count[i] = 0;
	
	//finding the optimal solution for the problem
	findOptimal(course_credits_sorted, course_credits_sorted_indexes, courses_taken_count, course_count, credit_count, course_count);
	
	//outputting the solution found
	outputResults(course_credits, courses_taken_count, course_count, credit_count);
	
	//pausing the program and ending it after that
	system("PAUSE");
	return 0;
}

void inputCreditCourseCounts(int *credit_count, int *course_count)
{
	while(1)
	{
		printf("Enter total number of credits: ");
		scanf("%d", credit_count);
		
		if(*credit_count >= 1)
			break;
		
		printf("Credit count must be bigger than or equal to 1.\n");
	}
	
	while(1)
	{
		printf("Enter number of courses having different credits: ");
		scanf("%d", course_count);
		
		if(*course_count >= 1)
			break;
		
		printf("Course count must be bigger than or equal to 1.\n");
	}
}

void inputCourseCreditsList(int course_credits[], int course_count)
{
	printf("Course credit for course #1 is 1, this cannot be changed.\n");
	course_credits[0] = 1;
	
	int i, j, input, flag;
	
	for(i = 1; i < course_count; i++)
	{
		while(1)
		{
			printf("Enter the course credit for course #%d: ", i+1);
			scanf("%d", &input);
			
			flag = 1;
			for(j = 0; j < i; j++)
			{
				if(input == course_credits[j])
				{
					flag = 0;
					break;
				}
			}
			
			if(flag)
			{
				course_credits[i] = input;
				break;
			}
			
			printf("You have already entered this amount of credit.\n");
		}
	}
}

void swapVariableValues(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyModified(int arr[], int arr_indexes[], int N, int i)
{
    int largest = i;
    int left = (2*i)+1;
    int right = (2*i)+2;
 	
    if(left < N && arr[left] > arr[largest])
        largest = left;
	
    if(right < N && arr[right] > arr[largest])
        largest = right;
	
    if(largest != i)
	{
        swapVariableValues(&arr[i], &arr[largest]);
        swapVariableValues(&arr_indexes[i], &arr_indexes[largest]);
        heapifyModified(arr, arr_indexes, N, largest);
    }
}

void heapSortModified(int arr[], int arr_indexes[], int N)
{
    for(int i = (N/2)-1; i >= 0; i--)
        heapifyModified(arr, arr_indexes, N, i);
 	
    for(int i = N-1; i >= 0; i--)
	{
        swapVariableValues(&arr[0], &arr[i]);
        swapVariableValues(&arr_indexes[0], &arr_indexes[i]);
        heapifyModified(arr, arr_indexes, i, 0);
    }
}

int findOptimal(int course_credits_sorted[], int course_credits_sorted_indexes[], int courses_taken_count[], int course_count, int credit_count, int course_count_fixed)
{
	//declaring variables
	int i, temp, courses_take_count_current[course_count_fixed], total_course_take_count = 0, total_course_take_count_previous = 2000000000;
	
	//creating course take count list for the current problem
	for(i = 0; i < course_count_fixed; i++)
	{
		courses_take_count_current[i] = 0;
	}
	
	//if there exists a smaller problem, then doing recursion 
	//to find the optimal solution of the smaller problem
	if(course_count > 1)
		total_course_take_count_previous = findOptimal(course_credits_sorted, course_credits_sorted_indexes, courses_taken_count, course_count-1, credit_count, course_count_fixed);
	
	//solving the current problem
	i = course_count-1;
	while(credit_count > 0)
	{
		temp = credit_count / course_credits_sorted[i];
		total_course_take_count += temp;
		credit_count -= temp * course_credits_sorted[i];
		courses_take_count_current[course_credits_sorted_indexes[i]] = temp;
		i--;
	}
	
	//comparing the result with the previous problems solution
	//if this solution is better, saving it
	//if not, continuing with the best result
	if(total_course_take_count_previous > total_course_take_count)
	{
		for(i = 0; i < course_count_fixed; i++)
			courses_taken_count[i] = courses_take_count_current[i];
	}
	else
	{
		total_course_take_count = total_course_take_count_previous;
	}
	
	//returning the best result
	return total_course_take_count;
}

void outputResults(int course_credits[], int courses_taken_count[], int course_count, int credit_count)
{
	int i, total_take_count = 0;
	
	printf("\n\n------------------------------------------------------------------\n");
	printf("You need to take:\n");
	for(i = 0; i < course_count; i++)
	{
		if(courses_taken_count[i] > 0)
		{
			printf("%d course(s) having %d credit.\n", courses_taken_count[i], course_credits[i]);
			total_take_count += courses_taken_count[i];
		}
	}
	printf("Minimum number of courses to be taken to satisfy %d credits is: %d\n", credit_count, total_take_count);
	printf("------------------------------------------------------------------\n\n");
}
