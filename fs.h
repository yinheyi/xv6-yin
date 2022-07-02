// On-disk file system format.
// Both the kernel and user programs use this header file.


#define ROOTINO 1  // root i-number
#define BSIZE 512  // block size

// Disk layout:
// [ boot block | super block | log | inode blocks |
//                                          free bit map | data blocks]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:
struct superblock {
  uint size;         // Size of file system image (blocks)
  uint nblocks;      // 整个文件系统中，用于保存实际数据的block块的数目
  uint ninodes;      // 整个文件系统中，inode的数目
  uint nlog;         // Number of log blocks
  uint logstart;     // Block number of first log block
  uint inodestart;   // inode所占磁盘空间的起始block块（即在第几个block块上开始是存放inode的地方)
  uint bmapstart;    //  BITMAP(用于标记每一个数据块的block是否可用)所占磁盘空间的起始block块。
};

#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof(uint))
#define MAXFILE (NDIRECT + NINDIRECT)

// On-disk inode structure
// 实际磁盘上保存的inode的数据结构
struct dinode {
  short type;              // inode的类型，包含：
  short major;             // Major device number (T_DEV only)
  short minor;             // Minor device number (T_DEV only)
  short nlink;             // Number of links to inode in file system
  uint size;               // 该inode对应的数据的大小
  uint addrs[NDIRECT+1];   // 该inode对应的数据的block块的位置. 前NDIRECT个block直接保存用户数据，
                           // 最后一个block里又保存了NDIRECT个block块的块号， 相当于两层,了，所以
                           // 文件最大为:512B * 12 + 512B * 12 = 12KB.
};

// Inodes per block.
#define IPB           (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i, sb)     ((i) / IPB + sb.inodestart)

// Bitmap bits per block
#define BPB           (BSIZE*8)

// Block of free map containing bit for block b
#define BBLOCK(b, sb) (b/BPB + sb.bmapstart)

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14

struct dirent {
  ushort inum;        // 目录或文件对应inode的磁盘上的索引号，即第几个inode。
  char name[DIRSIZ];  // 名字字符串
};

