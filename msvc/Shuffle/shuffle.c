
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include"rating.h"
#include<string.h>
#include<time.h>



int main(int argc,char** argv)
{
	unsigned int index = 0;
	FILE *input,*output;
	unsigned int ratings_number = atoi(argv[1]);
	rating_t* rating_list = (rating_t*)malloc(ratings_number*sizeof(rating_t));
	char* input_path = (char*)malloc(strlen(argv[2])+1);
	char* output_path = (char*)malloc(strlen(argv[3])+1);
	if(argc != 4){
		printf("Incorrect arguments \n");
		printf("usage: Shuffle 100000 ../input.data ../output.data " " \n kfold -h for help \n");
		if(strcmp(argv[1],"-h")){
			printf("Shuffle ratings_number input_file_path output_file_path \n");
			printf("ratings_number : Number of ratings in the sample \n");
			printf("input_file_path : the path of the samples file \n");
			printf("output_file_path : the path of the output file \n");
		}
	}

	strcpy(input_path,argv[2]);
	strcpy(output_path,argv[3]);
	input = fopen(input_path,"r");
	output = fopen(output_path,"w");
	assert(input);
	while (!feof(input)){
		if(fscanf(input, "%d::%d::%lf::%d", &rating_list[index].user, &rating_list[index].item, 
					&rating_list[index].rating, &rating_list[index].time_stamp) != 4 )
		{
			break;
		}
		index++;
	}
	srand(time(NULL));
	for(index = 0;index<ratings_number;index++){
		int a = rand()%(ratings_number-index);
		fprintf(output, "%d %d %1.1f %d \n", rating_list[a].user, rating_list[a].item, 
					rating_list[a].rating, rating_list[a].time_stamp);

		memcpy(&rating_list[a],&rating_list[ratings_number-index-1],sizeof(rating_t));

	}
	
	printf("File %s has been shuffled into %s \n",input_path,output_path);
	free(input_path);
	free(output_path);
	free(rating_list);
	system("pause");
	return 0;
}