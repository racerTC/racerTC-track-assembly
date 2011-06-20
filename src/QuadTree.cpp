#include "QuadTree.h"

/*

#include <cmath>

bool QuadTree::IsPowerOf2( int n ) {
	// obliczamy, do jakiej potegi nalezy podniesc 2, aby otrzymac n
	double x = log( static_cast<double>(number) ) / log( 2.0 );
	// jezeli czesc ulamkowa x == 0 -> n jest potega dwojki
	if( x - floor(x) == 0.0 )
		return true;
	else
		return false;
}

QuadTree::QuadTree( int width, int height, int cellSize ):
	TrackDataContainer( width, height, cellSize )
{
	int size;

	// zapewnia, iz zarowno szerokosc, jak i wysokosc, sa potegami dwojki
	if( !IsPowerOf2(width) || !IsPowerOf2(height) )
		throw QuadTreeException();

	// oblicza faktyczna wiekosc obszaru jaki opisuje QuadTree
	if( width == height ) size = width;
	else if( width > height ) size = width;
	else size = heigt;

	// oblicza maksymalna glebokosc drzewa:
	mMaxDepth = log10( cellSize ) / log10(2);

	// alokuje korzen:
	int split_point = size >> 1;
	mpTreeRoot = new Node( 0, split_point, split_point );
}


QuadTree::~QuadTree() {
	delete mpTreePoint;
}

QuadTree::Node::Node( int depth, int splitPointX, int splitPointY, Node* parent ):
	mDepth( depth )
,	mSplitPointX( splitPointX ), mSplitPointY( splitPointY )
,	mChildNW(0), mChildNE(0), mChildSE(0), mChildSW(0)
,	mParent( parent )
{}

QuadTree::Node::Node( int depth, int splitPointX, int splitPointY, const QuadTreeData& data, Node* parent = 0 ):
	mData( data )
,	mDepth( depth )
,	mSplitPointX( splitPointX ), mSplitPointY( splitPointY )
,	mChildNW(0), mChildNE(0), mChildSE(0), mChildSW(0)
,	mParent( parent )
{}


QuadTree::Node::~Node() {

}

void QuadTree::changeCellSize( int cellSize ) {

}

QuadTree::QuadTreeData& QuadTree::pickData( Node *node, int x, int y ) {
	if( node->mSplitPointX < x ) {
		if( node->mSplitPointY < y ) {
			// polnocny zachod
			if( node->mChildNW )
				return pickData( node->mChildNW, x, y );
			else
				return node->mData;
		} else {
			// polodniowy zachod
			if( node->mChildSW )
				return pickData( node->mChildSW, x, y );
			else
				return node->mData;
		}
	} else {
		if( node->mSplitPointY < y ) {
			// polnocny wschod
			if( node->mChildNE )
				return pickData( node->mChildNE, x, y );
			else
				return node->mData;
		} else {
			// poludniowy wschod
			if( node->mChildSE )
				return pickData( node->mChildSE, x, y );
			else
				return node->mData;
		}
	}
}

bool QuadTree::setData( Node *node, int x, int y, const Data& data ) {
	bool compact = false;

	if( node->mSplitPointX < x ) {
		if( node->mSplitPointY < y ) {
			// polnocny zachod
			if( node->mDepth == mMaxDepth ) {
				if( node->mData != data ) {
					node->mData = data;
					return true;
				} else
					return false;
			} else {
				if( !node->mChildNW ) {
					if( node->mData == data )
						return false;
					node->mChildNW = new Node(
							node->mDepth+1,
							node->mSplitPointX >> 1,
							node->mSplitPointY >> 1,
							node->mData,
							this
							);
				}
				compact = setData( node->mChildNW, x, y, data );
			}
		} else {
			// polodniowy zachod
			if( node->mChildSW )
				return pickData( node->mChildSW, x, y );
			else
				return node->mData;
		}
	} else {
		if( node->mSplitPointY < y ) {
			// polnocny wschod
			if( node->mChildNE )
				return pickData( node->mChildNE, x, y );
			else
				return node->mData;
		} else {
			// poludniowy wschod
			if( node->mChildSE )
				return pickData( node->mChildSE, x, y );
			else
				return node->mData;
		}
	}

	if( compact )
		return compact( node );
	else
		return false;
}

const QuadTree::QuadTreeData& QuadTree::pickData( int x, int y ) {
	return pickData( mpTreeRoot, x ,y );
}

void QuadTree::setData( int x, int y, const QuadTreeData& data ) {
	Node** child_node;
	Node* node = pickNode( mpTreeRoot, x ,y );

	if( node->mDepth == mMaxDepth ) {
		node->mData = data;
	} else {

	}
}

void QuadTree::setDefaultData( const QuadTreeData& data ) {
	mpTreeRoot->mData = data;
}

*/
