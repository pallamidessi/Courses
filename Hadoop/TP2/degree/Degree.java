

import java.io.IOException;
import java.util.StringTokenizer;
import java.util.List;
import java.util.ArrayList;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class Degree {

  public static class MapperOne
       extends Mapper<Object, Text, Text, Text>{

    private final static IntWritable one = new IntWritable(1);
    private Text word = new Text();

    public void map(Object key, Text value, Context context
                    ) throws IOException, InterruptedException {
      StringTokenizer itr = new StringTokenizer(value.toString(), "\n");
      while (itr.hasMoreTokens()) {
        String tmp = itr.nextToken();
        word.set(tmp);

        String[] vertices = tmp.split(" ");
        System.out.println(tmp);
        System.out.println(vertices[0]);
        System.out.println(vertices[1]);

        context.write(new Text(vertices[0]), word);
        context.write(new Text(vertices[1]), word);
      }
    }
  }

  public static class MapperTwo
       extends Mapper<Object, Text, Text, Text>{

    private final static IntWritable one = new IntWritable(1);
    private Text word = new Text();

    public void map(Object key, Text value, Context context
                    ) throws IOException, InterruptedException {
      StringTokenizer itr = new StringTokenizer(value.toString(), "\n");
      while (itr.hasMoreTokens()) {
        String tmp = itr.nextToken();

        String[] vertices = tmp.split("\t");
        String[] res = vertices[1].split(" ");
        System.out.println(tmp);
        System.out.println(res[0]);
        System.out.println(res[1]);

        context.write(new Text(vertices[0]), new Text(vertices[1]));
      }
    }
  }

  public static class ReduceTwo
       extends Reducer<Text, Text ,Text, Text> {
    private IntWritable result = new IntWritable();

    public void reduce(Text key, Iterable<Text> values,
                       Context context
                       ) throws IOException, InterruptedException {

      Text tmp = new Text("");
      for (Text val : values) {
        tmp = tmp + val;
      }

      context.write(new Text("d(" + key + ") = "), tmp);
    }
  }

  public static class ReduceOne
       extends Reducer<Text, Text ,Text, Text> {
    private IntWritable result = new IntWritable();

    public void reduce(Text key, Iterable<Text> values,
                       Context context
                       ) throws IOException, InterruptedException {
      int sum = 0;

      List<Text> cpy = new ArrayList<Text>();

      for (Text val : values) {
        sum += 1;
        cpy.add(val);
      }

      for (Text val : cpy) {
        context.write(val, new Text(key + " " + sum));
        System.out.println(val + "\t " + key + " " + sum);
      }
    }
  }

  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
    FileSystem fs = FileSystem.newInstance(conf);

    if (otherArgs.length != 2) {
      System.err.println("Usage: wordcount <in> <out>");
      System.exit(2);
    }

    Job job = new Job(conf, "phase 1");
    job.setJarByClass(Degree.class);

    job.setMapperClass(MapperOne.class);
    job.setReducerClass(ReduceOne.class);

    job.setMapOutputKeyClass(Text.class);
    job.setMapOutputValueClass(Text.class);

    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(Text.class);
    FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
    FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));

    job.waitForCompletion(true);
    args[0] = args[1];
    args[1] = args[1] + "2";

    System.out.println(args[0]);
    System.out.println(args[1]);

    Configuration conf2 = new Configuration();
    String[] otherArgs2 = new GenericOptionsParser(conf2, args).getRemainingArgs();
    FileSystem fs2 = FileSystem.newInstance(conf2);

    Job job2 = new Job(conf2, "phase 2");
    job2.setJarByClass(Degree.class);

    job2.setMapperClass(MapperTwo.class);
    job2.setReducerClass(ReduceTwo.class);

    job2.setMapOutputKeyClass(Text.class);
    job2.setMapOutputValueClass(Text.class);

    job2.setOutputKeyClass(Text.class);
    job2.setOutputValueClass(Text.class);

    FileInputFormat.addInputPath(job2, new Path(otherArgs2[0] + "/part-r-00000"));
    FileOutputFormat.setOutputPath(job2, new Path(otherArgs2[1]));

    job2.waitForCompletion(true);
  }
}
