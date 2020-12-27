import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;

public class SharedMem
{
    public static List<Thread> producers = new LinkedList<>();
    public static List<Thread> consumers = new LinkedList<>();

    public static int resourceNum = 0;
    public static AtomicInteger totalConsumedNum = new AtomicInteger();

    public static Semaphore mutex_empty;
    public static Semaphore mutex_full;
    public static Semaphore mutex;
    public static Semaphore mutex_producers[];
    public static Semaphore mutex_consumers[];

    public static int bufferSize;
    public static Queue<Integer> buffer = new LinkedList<>();

    public SharedMem(int bufferSize, int producersNum, int consumersNum, int resNum, int resBound)
    {
        this.bufferSize = bufferSize;
        for (int i = 1; i <= producersNum; i++)
            producers.add(new Producer("P" + i, resNum, resBound));
        for (int i = 1; i <= consumersNum; i++)
            consumers.add(new Consumer("C" + i));
        totalConsumedNum.set(0);
        resourceNum = resNum;
        mutex_empty = new Semaphore(bufferSize);
        mutex_full = new Semaphore(0);
        mutex = new Semaphore(1);
        mutex_producers = new Semaphore[producers.size()];
        mutex_consumers = new Semaphore[consumers.size()];
        for (int i = 0; i < mutex_producers.length; i++)
            mutex_producers[i] = new Semaphore(0);
        mutex_producers[0].release();
        for (int i = 0; i < mutex_consumers.length; i++)
            mutex_consumers[i] = new Semaphore(0);
        mutex_consumers[0].release();

        //System.out.println("  = mutex_empty:" + mutex_empty.availablePermits() + ",mutex_full:" + mutex_full.availablePermits());
    }

    public static void startAll()
    {
        /*for(int i=producers.size()-1;i>=0;i--)
        {
            producers.get(i).start();
            System.out.println("<!>" + producers.get(i).getName() + " started");
        }*/
        for (Thread producer : producers)
        {
            producer.start();
            System.out.println("<!>" + producer.getName() + " started");
        }
        for (Thread consumer : consumers)
        {
            consumer.start();
            System.out.println("<!>" + consumer.getName() + " started");
        }
    }

    public static void printBufferUsage()
    {
        System.out.println("  =buffer:" + buffer);
    }


}
