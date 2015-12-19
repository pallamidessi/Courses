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
            extends Mapper<Object, Text, IntWritable, Text> {
        private Text word = new Text();

        public void map(Object key, Text value, Context context
        ) throws IOException, InterruptedException {
            StringTokenizer itr = new StringTokenizer(value.toString(), "\n");
            while (itr.hasMoreTokens()) {
                String tmp = itr.nextToken();
                System.out.println(tmp);
                word.set(tmp);

                int bucket = word.getLength() / 10;
                System.out.println(bucket);
                IntWritable size = new IntWritable(bucket);

                context.write(size, word);
            }
        }
    }

    public static class LevensteinReducer
            extends Reducer<IntWritable, Text, Text, IntWritable> {

        private final static IntWritable result = new IntWritable(0);
        public int distanceThreshold = 5;

        public void reduce(IntWritable key, Iterable<Text> values,
                           Context context
        ) throws IOException, InterruptedException {
            int count = 0;
            Vector<Text> tmpValues = new Vector<Text>();
            for (Text val : values) {
                count++;
                tmpValues.add(val);
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
                    int distance = Levenstein.levensteinDistance(tmpValues.get(i).toString(), tmpValues.get(j).toString());
                    if (distance < distanceThreshold) {
                        distanceMatrix[i][j] = true;
                        matches[i] = matches[i] + 1;
                    } else {
                        distanceMatrix[i][j] = false; // Impossible distance
                    }
                }
            }

            for(int val: matches) {
                System.out.println(val);
            }

            printDebug(distanceMatrix, count, count);

            createDistinctClass(matches, context, tmpValues);
            greedyClassCreation(count, matches, distanceMatrix, tmpValues, context);
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

        private boolean isMatrixEmpty(boolean[][] mat, int size) {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if(mat[i][j]) {
                        return false;
                    }
                }
            }
            return true;
        }

        private void createDistinctClass (int[] matches, Context context, List<Text> values)
                throws IOException, InterruptedException {
            int cnt = 0;
            for(int value : matches) {
                if(value == 0 ) {
                    context.write(values.get(cnt), result);
                }
                cnt++;
            }
        }

        private int findMaxMatches(int[] matches) {
            int max = 0;
            int index = 0;
            if (matches.length == 0) {
                return -1;
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

        private void printDebug(boolean[][] distanceMatrix, int sizeRow, int sizeCol) {
            for (int i = 0; i < sizeRow; i++) {
                for (int j = 0; j < sizeCol; j++) {
                    System.out.print(distanceMatrix[i][j] + " ");
                }
                System.out.println();
            }
        }
        private void greedyClassCreation(int size, int[] matches, boolean[][] mat, List<Text> values, Context context)
                throws IOException, InterruptedException {
            int max = 0;

            while (!isMatrixEmpty(mat, size)) {
                max = findMaxMatches(matches);
                context.write(values.get(max), result);
                invalidateColumn(max, size, mat);
                invalidateRow(max, size, mat);
                recomputeMatches(matches, mat, size);
            }
        }

        public static class ClassifyReducer
                extends Reducer<Text, IntWritable, Text, Text> {


            public void reduce(Text key, IntWritable values,
                               Context context
            ) throws IOException, InterruptedException {
                Runtime r = Runtime.getRuntime();
                Process classifier = r.exec("");

                int returnCode = classifier.waitFor();

                Text result;

                if (returnCode == 0) {
                    result = new Text("pos");
                } else if (returnCode == 1) {
                    result = new Text("neg");
                } else {
                    result = new Text("null");
                }

                context.write(result, key);
            }
        }

        public static class StatReducer
                extends Reducer<Text, Text, Text, IntWritable> {
            private IntWritable result = new IntWritable();

            public void reduce(Text key, Iterable<IntWritable> values,
                               Context context
            ) throws IOException, InterruptedException {
                int sum = 0;
                for (IntWritable val : values) {
                    sum ++;
                }

                result.set(sum);
                context.write(key, result);

            }
        }


    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "Batch text analysis");
        job.setJarByClass(BatchTextAnalysis.class);
        job.setMapperClass(BucketMapper.class);
        //job.setCombinerClass(LevensteinReducer.class);
        job.setReducerClass(LevensteinReducer.class);
        job.setOutputKeyClass(IntWritable.class);
        job.setOutputValueClass(Text.class);
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
