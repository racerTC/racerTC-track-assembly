#ifndef QUADTREE_H
#define QUADTREE_H

#include "TrackData.h"
#include "TrackDataContainer.h"
#include <exception>

#ifdef DISABLED_CAUSE_NOT_USED

class QuadTree : public TrackDataContainer
{
protected:
	/**
	 * Wezel QuadTree. Usuniecie wezla powoduje usuniecie wszystkich wezlow,
	 * ktorych rodzicem jest ten wezel.
	 */
	struct Node {
		QuadTreeData mData;
		int mDepth;
		int mSplitPointX, mSplitPointY;
		Node* mChildNW, mChildNE, mChildSE, mChildSW;
		Node* mParent;

		Node( int depth, int splitPointX, int splitPointY, Node* parent = 0 );

		Node( int depth, int splitPointX, int splitPointY, const QuadTreeData& data, Node* parent = 0 );

		~Node();
	};

	static QuadTreeData& pickData( Node* node, int x, int y );
private:
	int mMaxDepth;
	Node* mpTreeRoot;
public:

	/**
	 * Konstruuje drzewo QuadTree o zadanych wymiarach. Wymiary drzewa musza byc potega
	 * dwojki.
	 * @param width Szerokosc drzewa. Musi byc potega dwojki
	 * @param height Wysokosc drzewa. Musi byc potega dwojki.
	 * @param cellSize Wiekosc najmniejszego obszaru opisywanego przez QuadTree.
	 * @throw QuadTreeException jezeli wymiary drzewa sa niepoprawne.
	 */
	QuadTree( int width, int height, int cellSize );

	~QuadTree();

	void changeCellSize( int cellSize );

	const QuadTreeData& pickData( int x, int y );

	void setData( int x, int y, const QuadTreeData& data );

	/**
	 * Ustawia zestaw danych domyslnie obowiazujacy w obszarze opisywanym przez QuadTree.
	 */
	void setDefaultData( const QuadTreeData& data );

	/**
	 * Pozwala sprawdzic, czy zadana liczba jest potega dwojki.
	 * @return <b>true</b>, jezeli istnieje taki x, ze 2^x=number.
	 */
	static bool IsPowerOf2( int n );

};

#endif

#endif // QUADTREE_H
