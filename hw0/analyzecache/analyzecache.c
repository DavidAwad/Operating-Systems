#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <string.h>
#include "analyzecache.h"

const int powers[20] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512,
	1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288};

void printTraversals(double *traversals, int count) {
	int i;
	for(i=0; i < count; i++) {
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
	for(i=0; i < 22; i++){
		if(i>20) {
			//TODO: panic
			break;
		}	
		/* keep a marker for the number of accesses we do and how long it takes to traverse a 'cache' that gradually gets larger */
		int accesses = 0 ;
		start = clock();
		for(k=0 ;k < sizeof(tempCache); k+=powers[i]){
			/* access the kth element based on the current stride */
			char temp = tempCache[k]; 
			accesses++;
		}
		/* measure the time that it took to iterate over the array */
		end = clock();
		cpu_time_used = ((double) (end - start));
		/* now we can measure the amount of time it took to perform this number of accesses on this array */
		traversals[i] = (cpu_time_used / accesses) ; 
		/* traversals is now an array of all of the times it took to perform gradually larger iterations over an array. We can see when these times start to even out */ 
	}
	int convergence = trendCheck(&traversals); 
	printTraversals( traversals, 21);
	printf("converged around %d\n", convergence);
	return ( powers[convergence] );
}

int trendCheck(double *traversals){
	puts("checking array of recorded times\n");
	int f;
	double diff; 
	for(f=1; f<sizeof(traversals); f++){
		if( traversals[f] &&  traversals[f-1] ){
			if( (diff = traversals[f] - traversals[f-1]) ){
				if( diff >  10 ){
					return f; 
				} 
			}
		}
	}
}
double caugeMiss(int cacheSize){
	printf("CAUGEMISS CALLED WITH SIZE : %d\n", cacheSize);
	char *line = calloc( (  sizeof(char)* (cacheSize + 5) ),  1 );
	int i,j,k,l; 
	double cpu_time_used=0, lastHit=0; 
	clock_t start, end;
	for(i=0;i<cacheSize+5;i++){
		line[i] = 'a'; 
	}
	return 67.000;  
	for(i=0 ;  i< ( cacheSize / 2 ); i++ ){
		line[i] = 'b' ; 
	}
	start = clock();
	for(i=0;i<20000;i++){
		for(j=0;j<10000;j++){
		}
	}
	line[cacheSize +3] = 'c'; 
	end = clock();
	cpu_time_used = ( (double)(end - start) );
	start = clock();
	for(i=0;i<20000;i++){
		for(j=0;j<10000;j++){
		}
	}
	end = clock();
	lastHit = ((double ) (end - start));

	printf("CPU TIME IS %lf\n", cpu_time_used); 
	
	return cpu_time_used - lastHit ; 
	//return cpu_time_used;
}

double causeMiss(int cacheSize){
	printf("MISSING a cacheSize which is %d\n", cacheSize) ;
	char *line = malloc(sizeof(char)*cacheSize); 
	int i,j;
	double cpu_time_used=0, lastHit=0; 
	clock_t start, end;
	/* run through array to load it into memory*/
	for(i=0; i<cacheSize; i++){
		char a = line[i]; 
	}
	for (i=0;i<cacheSize+1;i++){
		/* start clock */
		start = clock();
		for(j=0;j<cacheSize;j++){
		line[j] = 'b';
		}
		end = clock();
		/* end clock */ 
		cpu_time_used = ((double)(end - start)); //CLOCKS_PER_SEC;
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

int blockSize(int lineSize){
	int i,j,k;
	double cpu_time_used=0, lastTime=0;
	double timeMeasure [10737] ;
	clock_t start, end;
	int stride = lineSize; 
	for(i=1; i<10737; i++){
		/* keep stride constant and time interation through array until time gets huge.*/
		char *tempBlock = calloc(  (sizeof(char) * i ) , 1  ) ;
		for(j=0;j<i;j++){
			char temp = tempBlock[j];
		}
		start = clock();
		for(j=0;j<i;j+=stride){
			char temp = tempBlock[j];
		}
		end = clock();
		cpu_time_used = ((double)(end - start));
		timeMeasure[i] = cpu_time_used; 
	}	
	printTraversals(timeMeasure, 300); 
}

int main(int argc, char *argv[]){
	int lineSize = cacheLine(); 
	double cachePenalty = caugeMiss(lineSize); 
	int block = blockSize(lineSize) ;
	//int block = ( (blockSize(lineSize) ) / (1024 ) ) ;
	/* 
	printf("caugeMiss is penalty of %lf\n", caugeMiss(256)) ; 
	*/
	printf("Cache Block/Line Size: %d B\n", lineSize);
	printf("Cache Size: %d KB\n", block );
	printf("Cache Miss Penalty: %lf us\n", cachePenalty ); 
	return 0;
}
