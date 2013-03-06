
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include"rating.h"
#include<string.h>
#include<time.h>



int main(int argc,char** argv){
	unsigned int index=0;
	FILE *input,*output;
	unsigned int ratings_number=atoi(argv[1]);
	rating_t* rating_list=(rating_t*)malloc(ratings_number*sizeof(rating_list));
	char* input_path=(char*)malloc(sizeof(argv[2]));
	char* output_path=(char*)malloc(sizeof(argv[3]));
	strcpy(input_path,argv[2]);
	input=fopen(input_path,"r");
	output=fopen(output_path,"w");
	assert(input);

	while (!feof(input)){
		if (fscanf(input, "%d %d %lf %d", &rating_list[index].user, &rating_list[index].item, 
					&rating_list[index].rating, &rating_list[index].time_stamp) != 4)
			break;
		index++;
	}

	srand(time(NULL));
	for(index=0;index<ratings_number;index++){
		int a=rand()%ratings_number-index;
		if (fprintf(output, "%d %d %lf %d", rating_list[a].user, rating_list[a].item, 
					rating_list[a].rating, rating_list[a].time_stamp) != 4)
			break;
		memcpy(&rating_list[a],&rating_list[ratings_number-index],sizeof(rating_t));

	}
}