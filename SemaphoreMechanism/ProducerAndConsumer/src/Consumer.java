import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

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
                SharedMem.mutex_full.acquire();
                //System.out.println("  -(" + Thread.currentThread().getName() + "," + actionID + "): mutex_full:" + mutex_full.availablePermits() );
                SharedMem.mutex_consumers[SharedMem.consumers.indexOf(Thread.currentThread())].acquire();
                SharedMem.mutex.acquire();
                if (SharedMem.buffer.size() > 0)
                {
                    int n = SharedMem.buffer.remove();
                    data.add(n);
                    SharedMem.totalConsumedNum.addAndGet(1);
                    System.out.println(">>>[" + Thread.currentThread().getName() + "," + actionID + "]: consumed " + n);
                    printConsumedData();
                }
                SharedMem.mutex.release();
                SharedMem.mutex_consumers[(SharedMem.consumers.indexOf(Thread.currentThread()) + 1) % SharedMem.mutex_consumers.length].release();
                SharedMem.mutex_empty.release();
                //System.out.println("  -(" + Thread.currentThread().getName() + "," + actionID + "): mutex_empty:" + mutex_empty.availablePermits() );
                if (SharedMem.totalConsumedNum.get() >= SharedMem.resourceNum)
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
