import java.util.LinkedList;
import java.util.Queue;
import java.util.Random;
import java.util.concurrent.atomic.AtomicInteger;

class Producer extends Thread
{
    private static Queue<Integer> resource;
    private static int resourceSize = 0;
    private AtomicInteger actionID = new AtomicInteger();

    public static void geneResource(int resSize, int bound)
    {
        if (resource == null)
        {
            resource = new LinkedList<>();
            int sum = 0;
            resourceSize = resSize;
            for (int i = 0; i < resourceSize; i++)
            {
                int n = new Random().nextInt(bound);
                sum += n;
                resource.add(n);
            }
            System.out.println(resource.toString());
        }
    }

    public Producer(String name, int resNum, int resBound)
    {
        super(name);
        actionID.set(0);
        geneResource(resNum, resBound);
    }

    @Override
    public void run()
    {
        while (resource.size() > 0)
        {
            try
            {
                SharedMem.mutex_empty.acquire();
                //System.out.println("  -(" + Thread.currentThread().getName() + "," + actionID + "): mutex_empty:" + mutex_empty.availablePermits() );
                SharedMem.mutex_producers[SharedMem.producers.indexOf(Thread.currentThread())].acquire();
                SharedMem.mutex.acquire();
                if (resource.size() > 0)
                {
                    int data = resource.remove();
                    SharedMem.buffer.add(data);
                    System.out.println(">>>[" + Thread.currentThread().getName() + "," + actionID + "]: produced " + data);
                    SharedMem.printBufferUsage();
                }
                //sleep(100);
                SharedMem.mutex.release();
                SharedMem.mutex_producers[(SharedMem.producers.indexOf(Thread.currentThread()) + 1) % SharedMem.mutex_producers.length].release();
                SharedMem.mutex_full.release();
                //System.out.println("<!>Unlock "+producers.get((producers.indexOf(Thread.currentThread()) + 1) % producers.size()).getName());
                //System.out.println("  -(" + Thread.currentThread().getName() + "," + actionID + "): mutex_full:" + mutex_full.availablePermits() );
                actionID.addAndGet(1);
                //System.out.println("<!>Lock "+Thread.currentThread().getName());
            } catch (InterruptedException e)
            {
                e.printStackTrace();
            }
        }
        System.out.println("<!>" + Thread.currentThread().getName() + " stopped (resource used up)");
    }

}

