#include <stdlib.h>
#include <openssl/rand.h>
#include <math.h>

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

float **simPMF(int m, int n, float p){
  // 'm' is number of coin tosses, 'n' is number of trials, 'p' is probability of tail
  float *pts = (float *) malloc(sizeof(float)* (m + 1)); 
  for(int i = 0; i < m + 1; i++) pts[i] = 0;

  for(int i = 0; i < n; i++){
    // m' random tosses represented by Bernoulli random variable and their Cumulative sum
    int sum = 0;

    for(int j = 0; j < m; j++) sum += bernoulli(p);
    pts[sum] += 1;
  }

  // Normalizing
  for(int i = 0; i < m + 1; i++) pts[i] /= n;

  float **pts1 = (float **) malloc(sizeof(float *) * (m+1));
  for (int i = 0; i < m+1; i++){
    pts1[i] = (float *) malloc(sizeof(float) * 2);
    pts1[i][0] = i;
    pts1[i][1] = pts[i];
  }
  return pts1;
}

float **simCDF(int m, int n, float p){
  // 'm' is number of coin tosses, 'n' is number of trials, 'p' is probability of tail
  float *pts = (float *) malloc(sizeof(float *) * (m + 1)); 
  for(int i = 0; i < m + 1; i++) pts[i] = 0;

  for(int i = 0; i < n; i++){
    // Simulatiing 'm' random tosses with Bernoulli random variable
    // sum is the number of tails in this trial
    int sum = 0;
    for(int j = 0; j < m; j++) sum += bernoulli(p);

    // for each after random variable after 'sum', we add 1 for cummulating all the probabilities
    for(int j = m; j >= sum; j--) pts[j] += 1;
  }

  // Normalizing
  for(int i = 0; i < m + 1; i++) pts[i] /= n;

  float **pts1 = (float **) malloc(sizeof(float *) * (m+1));
  for (int i = 0; i < m+1; i++){
    pts1[i] = (float *) malloc(sizeof(float) * 2);
    pts1[i][0] = i;
    pts1[i][1] = pts[i];
  }

  return pts1;
}

/*
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
*/


// free a 2 dimentional array 'points' with 'n' rows in memory
void freeSingle(float **points, int n){
  for(int i = 0; i < n; i++){
    free(points[i]);
  }

  free(points);
}
