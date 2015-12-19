import java.io.IOException;
import java.util.*;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.MultipleOutputs;

public class BatchTextAnalysis {

    public static class BucketMapper
            extends Mapper<Object, Text, Text, IntWritable> {

        public void map(Object key, Text value, Context context
        ) throws IOException, InterruptedException {
            int bucket = value.getLength() / 10;
            IntWritable size = new IntWritable(bucket);
            context.write(value, size);
        }
    }

    public static class LevensteinReducer
            extends Reducer<Text, IntWritable, Text, IntWritable> {

        private final static IntWritable result = new IntWritable(0);
        private MultipleOutputs mos;
        public int distanceThreshold = 5;

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
            for (int i = 0; i < count; i++) {
                for (int j = 0; j < count; j++) {
                    distanceMatrix[i][j] = false;
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

            greedyClassCreation(count, matches, distanceMatrix);
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
            for (int i = 0; i < size; i++) {
                matches[i] = 0;

                for (int j = 0; j < size; j++) {
                    if (mat[i][j]) {
                        matches[i] = matches[i] + 1;
                    }
                }
            }
        }

        private int findMaxMatches(int[] matches) {
            int max = 0;
            int index = 0;
            if (matches.length == 0) {
                return index;
            }

            for (int i = 0; i < matches.length; i++) {
                if (i == 0) {
                    max = matches[0];
                    index = 0;
                }
                if (matches[i] > max) {
                    max = matches[i];
                    index = i;
                }
            }
            return index;
        }

        private void greedyClassCreation(int size, int[] matches, boolean[][] mat) {
            int max = 0;

            while ((max = findMaxMatches(matches)) > 0) {
                mos.write(values[max], result);
                invalidateRow(max, size, mat);
                invalidateColumn(max, size, mat);
                recomputeMatches(matches, mat, size);
            }
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "word count");
        job.setJarByClass(BatchTextAnalysis.class);
        job.setMapperClass(BucketMapper.class);
        job.setCombinerClass(LevensteinReducer.class);
        job.setReducerClass(LevensteinReducer.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
