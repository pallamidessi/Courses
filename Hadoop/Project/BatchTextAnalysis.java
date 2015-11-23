import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class BatchTextAnalysis {

  public static class BucketMapper
    extends Mapper<Object, Text, Text, IntWritable>{

      public void map(Object key, Text value, Context context
          ) throws IOException, InterruptedException {
        int bucket = value.length() / 10;
        IntWritable size = new IntWritable(bucket);
        context.write(value, bucket);
      }
    }

  public static class LevensteinReducer
    extends Reducer<Text,IntWritable,Text,IntWritable> {

      private final static IntWritable resukt = new IntWritable(0);
      private MultipleOutputs mos;

      public void reduce(Text key, Iterable<IntWritable> values,
          Context context
          ) throws IOException, InterruptedException {
        int count = 0;

        for (IntWritable val : values) {
          count++;
        }

        boolean[][] distanceMatrix = new boolean[count][count];
        int[] matches = new int[count];

        // Set matrix to false
        for(int i = 0; i < count; i++){
          for (int j = 0; j < count; j++) {
            distanceMatrix = false;
          }
        }

        // Compute Levenstein distance between every message of a bucket
        for (int i = 0; i < count; i++) {
          for (int j = 0; j < count; j++) {
            int distance = Levenstein.levensteinDistance(values[i], values[j]);
            if (distance < distanceThreshold) {
              distanceMatrix[i][j] = true;
              matches[i] = matches[i] + 1;
            } else {
              distanceMatrix[i][j] = false; // Impossible distance
            }
          }
        }

        greedyClassCreation();
      }

      private void invalidateColumn(int row, int size, boolean[][] mat) {
        // Invalidate in the matrix the use of a message for a class
        for (int j = 0; j < size; j++) {
          if (mat[row][j]) {
            for (int i = 0; i < size; i++) {
              mat[i][j] = false;
            }
          }
        }
      }

      private void invalidateRow(int row, int size, boolean[][] mat) {
        // Invalidate the selected message for a class
        for (int j = 0; j < size; j++) {
          mat[row][j] = false;
        }
      }

      private void recomputeMatches(int[] matches, boolean[][] mat, int size) {
        for(int i = 0; i < size; i++){
          matches[i] = 0;

          for(int j = 0; j < size; j++) {
            if (mat[i][j]) {
              matches[i] = matches[i] + 1;
            }
          }
        } 
      }

      private int findMaxMatches(int[] matches) {
        List tmpList = Arrays.asList(ArrayUtils.toObject(matches));
        int max = Collections.max();

        return max;
      }

      private greedyClassCreation(int size, int[] matches, boolean[][] mat) {
        int max = 0;

        while((max = findMaxMatches(matches)) > 0) {
          mos.write(values[max], result);
          invalidateRow(max, size, distanceMatrix);
          invalidateColumn(max, size, distanceMatrix);
          recomputeMatches(matches, distanceMatrix, size);
        }
      }


      public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "word count");
        job.setJarByClass(WordCount.class);
        job.setMapperClass(BucketMapper.class);
        job.setCombinerClass(IntSumReducer.class);
        job.setReducerClass(IntSumReducer.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
      }
    }
