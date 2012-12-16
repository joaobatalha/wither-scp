#ifndef BITMAP_HH
#define BITMAP_HH

class Bitmap {
    private:
        char bitmap_[300000];
        int num_blocks_; 

        bool is_set( int block_number );
    public:
        Bitmap(int n_blocks);

        bool is_full( void );

        void set_bit( int block_number );

        int next_block( void );
};
#endif
