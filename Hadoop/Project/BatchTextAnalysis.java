import java.io.IOException;
import java.util.*;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.mapreduce.lib.output.MultipleOutputs;
import org.apache.hadoop.util.GenericOptionsParser;

public class BatchTextAnalysis {


    public static class IdemMapperLevenstein
            extends Mapper<Object, Text, Text, IntWritable> {

        private final static IntWritable result = new IntWritable(0);

        public void map(Object key, Text value, Context context
        ) throws IOException, InterruptedException {
            context.write(value, result);
        }
    }


    public static class IdemMapperClassifier
            extends Mapper<Object, Text, Text, NullWritable> {

        private final static NullWritable result = NullWritable.get();

        public void map(Object key, Text value, Context context
        ) throws IOException, InterruptedException {

            StringTokenizer itr = new StringTokenizer(value.toString(), "\n");
            while (itr.hasMoreTokens()) {
                String tmp = itr.nextToken();

                String[] splittedLine = tmp.split("\t");


                context.write(new Text(splittedLine[0]), result);
            }
        }
    }


    public static class BucketMapper
            extends Mapper<Object, Text, IntWritable, Text> {
        private Text word = new Text();

        public void map(Object key, Text value, Context context
        ) throws IOException, InterruptedException {
            StringTokenizer itr = new StringTokenizer(value.toString(), "\n");
            while (itr.hasMoreTokens()) {
                String tmp = itr.nextToken();
                word.set(tmp);

                int bucket = word.getLength() / 10;
                IntWritable size = new IntWritable(bucket);

                context.write(size, word);
            }
        }
    }

    public static class LevensteinReducer
            extends Reducer<IntWritable, Text, Text, NullWritable> {

        private final static NullWritable result = NullWritable.get();
        public int distanceThreshold = 2;

        public void reduce(IntWritable key, Iterable<Text> values,
                           Context context
        ) throws IOException, InterruptedException {
            int count = 0;
            Vector<String> tmpValues = new Vector<String>();

            for (Text val : values) {
                count++;
                tmpValues.add(val.toString());
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
                    int distance = Levenstein.levensteinDistance(tmpValues.get(i), tmpValues.get(j));
                    if (distance < distanceThreshold * (key.get() + 1)) {
                        distanceMatrix[i][j] = true;
                        matches[i] = matches[i] + 1;
                    } else {
                        distanceMatrix[i][j] = false; // Impossible distance
                    }
                }
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
                    if (mat[i][j]) {
                        return false;
                    }
                }
            }
            return true;
        }

        private void createDistinctClass(int[] matches, Context context, List<String> values)
                throws IOException, InterruptedException {
            int cnt = 0;
            for (int value : matches) {
                if (value == 0) {
                    context.write(new Text(values.get(cnt)), result);
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

        private void greedyClassCreation(int size, int[] matches, boolean[][] mat, List<String> values, Context context)
                throws IOException, InterruptedException {
            int max = 0;

            while (!isMatrixEmpty(mat, size)) {
                max = findMaxMatches(matches);
                context.write(new Text(values.get(max)), result);
                invalidateColumn(max, size, mat);
                invalidateRow(max, size, mat);
                recomputeMatches(matches, mat, size);
            }
        }
    }

    public static class ClassifyReducer
            extends Reducer<Text, IntWritable, Text, Text> {

        @Override
        public void reduce(Text key, Iterable<IntWritable> values,
                           Context context
        ) throws IOException, InterruptedException {
            Runtime r = Runtime.getRuntime();
            Process classifier = r.exec(System.getProperty("user.home") + "/.virtualenvs/twitter/bin/python2.7 trained_classifier/classify.py " + key.toString());


            int returnCode = classifier.waitFor();

            Text result;

            if (returnCode == 0) {
                result = new Text("neg");
            } else if (returnCode == 1) {
                result = new Text("pos");
            } else {
                result = new Text("null");
            }

            context.write(result, key);
        }
    }

    public static class StatReducer
            extends Reducer<Text, NullWritable, Text, IntWritable> {
        private IntWritable result = new IntWritable();

        @Override
        public void reduce(Text key, Iterable<NullWritable> values,
                           Context context
        ) throws IOException, InterruptedException {
            int sum = 0;

            for (NullWritable val : values) {
                sum++;
            }

            result.set(sum);
            context.write(key, result);

        }
    }


    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();


        Job job = new Job(conf, "Batch text analysis Levenstein");
        job.setJarByClass(BatchTextAnalysis.class);
        job.setMapperClass(BucketMapper.class);
        //job.setCombinerClass(LevensteinReducer.class);
        job.setReducerClass(LevensteinReducer.class);
        job.setOutputKeyClass(IntWritable.class);
        job.setOutputValueClass(Text.class);
        FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
        FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
        job.waitForCompletion(true);

        args[0] = args[1];
        args[1] = args[1] + "2";

        Configuration conf2 = new Configuration();
        String[] otherArgs2 = new GenericOptionsParser(conf2, args).getRemainingArgs();


        Job job2 = new Job(conf2, "Batch text analysis Levenstein Classifier");
        job2.setJarByClass(BatchTextAnalysis.class);

        job2.setMapperClass(IdemMapperLevenstein.class);
        job2.setReducerClass(ClassifyReducer.class);

        job2.setMapOutputKeyClass(Text.class);
        job2.setMapOutputValueClass(IntWritable.class);

        job2.setOutputKeyClass(Text.class);
        job2.setOutputValueClass(Text.class);

        FileInputFormat.addInputPath(job2, new Path(otherArgs2[0] + "/part-r-00000"));
        FileOutputFormat.setOutputPath(job2, new Path(otherArgs2[1]));

        job2.waitForCompletion(true);

        args[0] = otherArgs[1] + "2";
        args[1] = otherArgs[1] + "3";

        Configuration conf3 = new Configuration();
        String[] otherArgs3 = new GenericOptionsParser(conf3, args).getRemainingArgs();

        Job job3 = new Job(conf2, "Batch text analysis Stat Classifier");
        job3.setJarByClass(BatchTextAnalysis.class);

        job3.setMapperClass(IdemMapperClassifier.class);
        job3.setReducerClass(StatReducer.class);

        job3.setMapOutputKeyClass(Text.class);
        job3.setMapOutputValueClass(NullWritable.class);

        job3.setOutputKeyClass(Text.class);
        job3.setOutputValueClass(IntWritable.class);

        FileInputFormat.addInputPath(job3, new Path(otherArgs3[0] + "/part-r-00000"));
        FileOutputFormat.setOutputPath(job3, new Path(otherArgs3[1]));

        job3.waitForCompletion(true);
    }
}
