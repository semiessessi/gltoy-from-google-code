
// Thomas Young

#ifndef __SRC_MAP_H_
#define __SRC_MAP_H_

class SRC_Map_Block
{
	// These are the blocks which make up a level

	public:

		SRC_Map_Block();
		~SRC_Map_Block();


};

class SRC_Map
{
	public:

	    SRC_Map();
		~SRC_Map();
		
		bool Load( char* szFilename );


	private:

		SRC_Map_Block* m_pxBlocks;

};

#endif

//eof
