package v3;

import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.LockSupport;

import static v3.SharedMem.*;

class Consumer extends Thread
{
    private List<Integer> data = new LinkedList<>();
    private AtomicInteger actionID = new AtomicInteger();

    public Consumer(String name)
    {
        super(name);
        actionID.set(0);
    }

    public int sum()
    {
        int sum = 0;
        for (Integer integer : data)
            sum += integer;
        return sum;
    }

    public void printConsumedData()
    {
        System.out.println("  =" + Thread.currentThread().getName() + " consumed data:" + data + " (sum:" + sum() + ")");
    }

    @Override
    public void run()
    {
        while (true)
        {
            try
            {
                mutex_full.acquire();// full信号量减1
                //System.out.println("  -(" + Thread.currentThread().getName() + "," + actionID + "): mutex_full:" + mutex_full.availablePermits() );
                mutex_consumers[consumers.indexOf(Thread.currentThread())].acquire();
                mutex.acquire();
                if (buffer.size() > 0)
                {
                    int n = buffer.remove();
                    data.add(n);
                    totalConsumedNum.addAndGet(1);
                    System.out.println(">>>[" + Thread.currentThread().getName() + "," + actionID + "]: consumed " + n);
                    printConsumedData();
                }
                mutex.release();
                mutex_consumers[(consumers.indexOf(Thread.currentThread()) + 1) % mutex_consumers.length].release();
                mutex_empty.release();// empty信号量加1
                //System.out.println("  -(" + Thread.currentThread().getName() + "," + actionID + "): mutex_empty:" + mutex_empty.availablePermits() );
                if (totalConsumedNum.get() >= resourceNum)
                {
                    System.out.println("<!>" + Thread.currentThread().getName() + " stopped (consumed all resources)");
                    System.exit(0);
                }
                actionID.addAndGet(1);
            } catch (InterruptedException e)
            {
                e.printStackTrace();
            }
        }
        //System.out.println("<!>" + Thread.currentThread().getName() + " stopped (consumed all resources)");
    }
}
