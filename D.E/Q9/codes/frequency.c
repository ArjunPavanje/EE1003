#include <stdlib.h>
#include <openssl/rand.h>

// Generating a random number between 0, 1
float random_gen(){
  // unsigned char data type is 256 bytes
  unsigned char buf;

  // RAND_BYTES generates random number and returns (1 if random number is generated succesfully, 0 otherwise)
  if (RAND_bytes(&buf, 1) != 1) {
        printf("Error generating random byte.");
        exit(-1);
    } 
  // Normalize it so that it is between 0, 1
  return (float) (buf)/255;
}

// Generates a bernoulli random variable,

int bernoulli(float p){
  return random_gen()<p?0:1; // if generated number is less than p, return 0 else return 1 
  // 0 represents heads, 1 represents tails
}

float **GetFreq(int n, int m, float p){
    // c is number of successful events

    float **pts = (float **) malloc(sizeof(float *) * n); 
    int c = 0;

    for(int i = 0; i < n; i++){
        pts[i] = (float *) malloc(sizeof(float) * 2);

        // We must account for i = 0 case seperately, as division by 0 will occur
        if(i == 0){
            pts[0][0] = 0;
            pts[0][1] = 1;
            continue;
        }

        // Simulating 'm' tosses 
        int *tosses = (int *) malloc(sizeof(int) * m);
        for(int j = 0; j < m; j++) tosses[j] = bernoulli(p);
        
        // Favoured case is when all tosses are 1 i.e. tails

        int isFavourable = 1;

        for(int j = 0; j < m; j++) {
            if(tosses[j] == 0){
                isFavourable = 0;
                break;
            }
        }

        // if  trial is favourable then count increases
        if(isFavourable) c++;
        free(tosses);

        pts[i][0] = i;
        pts[i][1] = ((float) c/i);
    }

    return pts;
}

// free a 2 dimentional array 'points' with 'n' rows in memory
void freeMultiMem(float **points, int n){
    for(int i = 0; i < n; i++){
        free(points[i]);
    }

    free(points);
}
