public class ProducerConsumer
{

    public static void main(String args[])
    {
        int bufferSize = 5, resourceSize = 10;
        int producersNum = 2, consumersNum = 2;
        int resourceBound = 100;
        SharedMem sharedMem = new SharedMem(bufferSize, producersNum, consumersNum, resourceSize, resourceBound);
        SharedMem.startAll();
    }
}

