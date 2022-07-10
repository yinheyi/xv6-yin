struct buf {
  int flags;             // 标志位，b_valid和b_dirty.
  uint dev;              // 缓存块对应的磁盘设备号
  uint blockno;          // 缓存块对应的block块号
  struct sleeplock lock; // 睡眠锁， 保证一个buffer同一时间只可能被一个进程拥有
  uint refcnt;           // 引用次数
  struct buf *prev;      // LRU双向链表
  struct buf *next;      // LRU双向链表
  struct buf *qnext;     // 当Buffer块需要与磁盘间进行同步时，Buffer块之间组成的单向同步队列
  uchar data[BSIZE];     // 512字节的数据缓存区
};
#define B_VALID 0x2  // buffer has been read from disk
#define B_DIRTY 0x4  // buffer needs to be written to disk

