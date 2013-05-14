#ifndef BITMAP_HH
#define BITMAP_HH

class Bitmap {
    private:
        int num_blocks_; 
        int just_sent_; 
        char bitmap_[300000];

        bool is_set( int block_number );
    public:
        Bitmap(int n_blocks);

        bool is_full( void );

        void set_bit( int block_number );

        int next_block( void );

        int num_blocks( void ) {return num_blocks_; }
};
#endif
