#include <stdio.h>
#include <stdlib.h>
#include "analyzecache.h"
#include <time.h> 
#include <string.h>
// 

const int powers[20] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512,
	1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288,};

void printTraversals(double *traversals, int count) {
	int i;

	for(i=0; i<count; i++) {
		printf("%lf\t", traversals[i]);
	}
	printf("\n");

}

int cacheLine(){
	clock_t start, end; 
	double cpu_time_used;
	char tempCache[ 16 * 1024 ] ; 
	int i=0,k,j;
	puts("cacheLine");
	/* Traverse array to load it into main memory, to then be read from the cache */ 
	for(k=0; k < sizeof(tempCache);k++ ){
		tempCache[k] = 'a'; 
	}
	double traversals[20] ;
	memset(traversals, 0, 50);
	/* now look through the array to analyze running time*/
	for(i=0; i < 22; i++) {

		if(i>20) {
			//TODO: panic
			fprintf(stderr, "i too large\n");
			break;
		}	

		//printf("i:%d\n", i);
		/*we're going to access every i'th thing to analyze runtime */

		int accesses;

		start = clock();
		for(j=0;j<10000000;j++){
			if(3>6){
				print("%d", 44 - 36 );
			}
		}

		accesses = 0;
		for(k=0 ;k < sizeof(tempCache); k+=powers[i]){
			tempCache[k]='b' ; 
			accesses++;
		}
		end = clock();
		cpu_time_used = ((double) (end - start));

		// now we have the time it took to traverse the array 
		//traversals[i] = (cpu_time_used / accesses) ; 
		traversals[i] = (cpu_time_used / powers[i]  ) ; 
		/* traversals is now an array of all of the times it took to perform the same number of accesses on a chunk  of data. We can see when that started to even out */ 
	}
	int convergence = trendCheck(&traversals); 
	printTraversals(traversals, 21);
	printf("converged around  %d\n", convergence);
	return ( powers[convergence] );

}

int trendCheck(double *traversals){
	puts("checking array");
	int f;
	double diff; 
	for(f=1; f<sizeof(traversals); f++){
		if( traversals[f] &&  traversals[f-1] ){
			if( (diff = traversals[f] - traversals[f-1]) ){
				if(diff < 5){
					return f; 
				} 
			}
		}
	}
}

double causeMiss(int cacheSize){
	char *line = malloc(sizeof(char)*cacheSize); 
	int i,j;
	double cpu_time_used=0, lastHit=0; 
	clock_t start, end;
	for(i=0; i<cacheSize; i++){
		line[i] = 'a'; 
	}

	for (i=0;i<cacheSize+1;i++){
		/* start clock */
		start = clock();
		for(j=0;j<100000;j++){
			if(3>2){
				line[i] = 'b';
			}
			line[i] = 'b';
		}
		end = clock();
		/* end clock */ 
		cpu_time_used = ((double)(end - start)); // CLOCKS_PER_SEC;
		//printf("cpu time is %lf",cpu_time_used);
		if(lastHit > 0){
			printf("bigger than zero\n");
			if( lastHit < cpu_time_used ){
				printf("cachemiss at %d ", i ); 
				return cpu_time_used ;
			}
		}
		else{   // first iteration
			lastHit = cpu_time_used;
		}
	}
	return cpu_time_used; 
}



/* Bad Solution */
int CacheSize(){
	int size = 0 ;
	clock_t start, end;
	double cpu_time_used;
	int i,k;
	while(1){	
		for(i=0 ; i < size ;i++){
			char tempCache[ size ];
			/* run through the array once to load into memory */
			for(k=0;k<size;k++){
				tempCache[k] = 'a' ;
			}
			double lastHit;
			/* now measure the time for each access */ 
			for(k=0;k<size;k++){
				start = clock();
				tempCache[k]='b' ;	
				end = clock();
				cpu_time_used = ((double) (end - start)); // CLOCKS_PER_SEC;
				if(lastHit){
					if( 2 * lastHit < cpu_time_used ){
						//cachePenalty = cpu_time_used ;
						//printf("CACHE MISS, cache is size %d and the penalty was %lf\n", size , cpu_time_used ); 
						return size; 
					}
				}
				else{ // first iteration
					lastHit = cpu_time_used ;
				}
			}
		}
		/* set a breakpoint to stop the while loop */ 
		size++ ;
		if( size > 40000  ){
			printf("the size is 1000!! breaking.");
			break; 
		}
	}
	printf("size is %d the time elapsed is %lf \n", size, cpu_time_used);
	return 0;
}


int main(int argc, char *argv[]){
	puts("main is here! \n");
	int cacheSize = cacheLine(); 
	double cachePenalty = causeMiss(cacheSize); 
	printf("Cache Block/Line Size: %d B\n",cacheSize);
	printf("Cache Size: XXX KB\n" );
	printf("Cache Miss Penalty: %lf us\n", cachePenalty ); /* given in units of microseconds */
	return 0;
}


