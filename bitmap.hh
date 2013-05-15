#ifndef BITMAP_HH
#define BITMAP_HH

class Bitmap {
    private:
        int num_blocks_; 
        int just_sent_; 
        int bits_set_; 
        char bitmap_[30000];

    public:
        Bitmap(int n_blocks);

        bool is_set( int block_number );

        bool is_full( void );

        void set_bit( int block_number );

        int next_block( void );

        int num_blocks( void ) {return num_blocks_; }
        
        int bits_set( void ) {return bits_set_; }
};
#endif
