//
//  stats.c
//
//  Author: David Meisner (meisner@umich.edu)
//

#include "stats.h"
#include "loader.h"
#include <assert.h>
#include "worker.h"

/**
 * Add this part to adjust the load
 * @Vinicius
 */
// Max number of steps we run
#define MAX_STEPS 300
/**
 * Add this part to adjust the load
 * @Vinicius
 */

pthread_mutex_t stats_lock = PTHREAD_MUTEX_INITIALIZER;

void addSample(struct stat* stat, float value) {
  stat->s0 += 1.0;
  stat->s1 += value;
  stat->s2 += value*value;
  stat->min = fmin(stat->min,value);
  stat->max = fmax(stat->max,value);

  if(value < .001){
    int bin = (int)(value*10000000);
    stat->micros[bin] += 1;
  } else if( value < 5.0){
    int bin = value * 10000.0;
    assert(bin < 50001);
    stat->millis[bin] += 1;
  } else if (value < 999){
    int bin = (int)value;
    stat->fulls[bin] += 1;
  } else {
    int bin = (int)value/1000;
    if (bin > 999){
      bin = 999;
    }
    stat->fulls[bin] += 1;
  }


}//End addAvgSample()

double getAvg(struct stat* stat) {
  return (stat->s1/stat->s0);
}//End getAvg()

double getStdDev(struct stat* stat) {
  return sqrt((stat->s0*stat->s2 - stat->s1*stat->s1)/(stat->s0*(stat->s0 - 1)));
}//End getStdDev()

//Should we exit because time has expired?
void checkExit(struct config* config) {

  int runTime = config->run_time;
  struct timeval currentTime;
  gettimeofday(&currentTime, NULL);
  double totalTime = currentTime.tv_sec - start_time.tv_sec + 1e-6*(currentTime.tv_sec - start_time.tv_sec);
  if(totalTime >= runTime && runTime >0) {
    printf("Ran for %f, exiting\n", totalTime);
    exit(0);
  }

}//End checkExit()

double findQuantile(struct stat* stat, double quantile) { 

  //Find the 95th-percentile
  int nTillQuantile = global_stats.response_time.s0 * quantile;
  int  count = 0;
  int i;
  for( i = 0; i < 10000; i++) {
    count += stat->micros[i];
    if( count >= nTillQuantile ){
      double quantile = (i+1) * .0000001;
      return quantile;
    }
  }//End for i

  for( i = 0; i < 50000; i++) {
    count += stat->millis[i];
    if( count >= nTillQuantile ){
      double quantile = (i+1) * .0001;
      return quantile;
    }
  }//End for i
  printf("count  %d\n", count);

  for( i = 0; i < 1000; i++) {
    count += stat->fulls[i];
    if( count >= nTillQuantile ){
      double quantile = i+1;
      return quantile;
    }
  }//End for i
  return 1000;

}//End findQuantile()

void printGlobalStats(struct config* config) {

  pthread_mutex_lock(&stats_lock);
  struct timeval currentTime;
  gettimeofday(&currentTime, NULL);
  double timeDiff = currentTime.tv_sec - global_stats.last_time.tv_sec + 1e-6*(currentTime.tv_sec - global_stats.last_time.tv_sec);
  double rps = global_stats.requests/timeDiff;
  double std = getStdDev(&global_stats.response_time);
  double q90 = findQuantile(&global_stats.response_time, .90);
  double q95 = findQuantile(&global_stats.response_time, .95);
  double q99 = findQuantile(&global_stats.response_time, .99);



  time_t tmp = (time_t)currentTime.tv_sec;

  /* char* timestamp = ctime(&tmp); */

  char timestamp[80];
  struct tm ts;
  ts = *gmtime(&tmp);
  strftime(timestamp, sizeof(timestamp), "%F %R:%S%z", &ts);

  printf("%11s, %10s,%8s,%16s, %8s,%11s,%10s,%13s,%10s,%10s,%10s,%12s,%10s,%10s,%11s,%14s\n", "timestamp", "timeDiff", "rps", "requests", "gets", "sets",  "hits", "misses", "avg_lat", "90th", "95th", "99th", "std", "min", "max", "avgGetSize");
  printf("%26s, %10f, %9.1f,  %10d, %10d, %10d, %10d, %10d, %10f, %10f, %10f, %10f, %10f, %10f, %10f, %10f\n", 
		timestamp, timeDiff, rps, global_stats.requests, global_stats.gets, global_stats.sets, global_stats.hits, global_stats.misses,
		1000*getAvg(&global_stats.response_time), 1000*q90, 1000*q95, 1000*q99, 1000*std, 1000*global_stats.response_time.min, 1000*global_stats.response_time.max, getAvg(&global_stats.get_size));
  
  int i;
  printf("Outstanding requests per worker:\n");
  for(i=0; i<config->n_workers; i++){
    printf("%d ", config->workers[i]->n_requests);
  } 
  printf("\n");
  fflush(stdout);
  //Reset stats
  memset(&global_stats, 0, sizeof(struct memcached_stats));
  global_stats.response_time.min = 1000000;
  global_stats.last_time = currentTime;

  checkExit(config);
  pthread_mutex_unlock(&stats_lock);

}//End printGlobalStats()

void setRPS(struct config* config, int new_rps) {
  int meanInterarrival = 1.0/(((float)new_rps)/(float)config->n_workers)*1e6;
  if (config->arrival_distribution_type == ARRIVAL_CONSTANT) {
    config->interarrival_dist = createConstantDistribution(meanInterarrival);
  } else {
    config->interarrival_dist = createExponentialDistribution(meanInterarrival);
  }
}

//Print out statistics every second
void statsLoop(struct config* config) {

  pthread_mutex_lock(&stats_lock);
  gettimeofday(&start_time, NULL);
  pthread_mutex_unlock(&stats_lock);

  if (config->dynamic_rps) {
    FILE* fp = NULL;
    if ((fp = fopen("load.cfg", "r+")) == NULL) {
      printf("There isn't any load configuration file: load.cfg\n");
      exit(1);
    }
    int time;
    int cumulative_time[MAX_STEPS];
    int qps_load[MAX_STEPS];
    int i = 0, cumulative = 0;
    while (fscanf(fp, "%d,%d\n", &time, &qps_load[i]) != EOF) {
      cumulative += time;
      cumulative_time[i] = cumulative;
      i++;
    }
    int total = i;
    fclose(fp);

    setRPS(config, qps_load[0]);

    sleep(2);
    printf("Stats:\n");
    printf("-------------------------\n");

    i = cumulative = 0;

    while(i < total) {
      printf("%d, %d: %d, %d\n", i, cumulative, cumulative_time[i], qps_load[i]);
      if (cumulative == cumulative_time[i]) {
        i++;
        setRPS(config, qps_load[i]);
      }
      cumulative += config->stats_time;
      printGlobalStats(config);
      sleep(config->stats_time);
    }
  } else {
    sleep(2);
    printf("Stats:\n");
    printf("-------------------------\n");
    while(1) {
      printf("%d, %d: %d, %d\n", 0, 0, 0, config->rps);
      printGlobalStats(config);
      sleep(config->stats_time);
    }
  }


}//End statisticsLoop()

