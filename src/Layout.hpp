#ifndef TERMBOXWIDGETS_LAYOUT_HPP
#define TERMBOXWIDGETS_LAYOUT_HPP

// NOTE: Not yet implemented. The goal is to provide basic automatic layout with tiling window algorithm

class Layout
{
	struct Property
	{
		float size;
	};

	struct Node : public Property
	{
		std::vector<Node*> children;
	};

	struct Element : public Property
	{

	};
};

#endif // TERMBOXWIDGETS_LAYOUT_HPP
