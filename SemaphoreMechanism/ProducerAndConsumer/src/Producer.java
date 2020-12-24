package v3;

import java.util.LinkedList;
import java.util.Queue;
import java.util.Random;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.LockSupport;

import static v3.SharedMem.*;

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
                mutex_empty.acquire();// empty信号量减1
                //System.out.println("  -(" + Thread.currentThread().getName() + "," + actionID + "): mutex_empty:" + mutex_empty.availablePermits() );
                mutex_producers[producers.indexOf(Thread.currentThread())].acquire();
                mutex.acquire();
                if (resource.size() > 0)
                {
                    int data = resource.remove();
                    buffer.add(data);
                    System.out.println(">>>[" + Thread.currentThread().getName() + "," + actionID + "]: produced " + data);
                    printBufferUsage();
                }
                //sleep(100);
                mutex.release();
                mutex_producers[(producers.indexOf(Thread.currentThread()) + 1) % mutex_producers.length].release();
                mutex_full.release();// full信号量加1
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

