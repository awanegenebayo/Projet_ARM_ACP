//  original source of BuildCgalPolyhedronFromObj downloaded from:
//  http://saurabhg.com/programming/wavefront-obj-reader-building-cgalpolyhedron_3/
//  ImportOBJ.h , TestImportOBJ.cpp

#include <CGAL/basic.h>
#include <CGAL/Modifier_base.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Simple_cartesian.h>

#include <fstream>
#include <iostream>

template <class HDS>
class PolyhedronBuilder : public CGAL::Modifier_base<HDS> {
private:
  std::istream& _file;
public:
  PolyhedronBuilder(std::istream& file) : _file(file) {}
  void
  operator() (HDS& hds)
  {
    typedef typename HDS::Vertex   Vertex;
    typedef typename Vertex::Point Point;
    if(!_file){ return;}
    std::string _line;
    int _numVertices = 0;
    int _numFacets   = 0;
    while(_file.good())
      {
	std::getline(_file, _line);
	if(_line.size() > 1)
	  {
	    if(_line[0]=='v' && _line[1]==' ') {++_numVertices;}
	    if(_line[0]=='f' && _line[1]==' ') {++_numFacets;}
	  }
      }
    if(!_file.good()){ _file.clear();}
    _file.seekg(0);
    // Postcondition: hds is a valid polyhedral surface.
    CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
    B.begin_surface(_numVertices, _numFacets, int((_numVertices + _numFacets - 2)*2.1));
    std::string _token;
    while(!_file.eof())
      {
	_token = ""; // Reset token.
	_file >> _token;
	// if token is v then its a vertex.
	if(_token=="v")
	  {
	    double x, y, z;
	    _file >> x >> y >> z;
	    B.add_vertex(Point(x, y, z));
	  }
	// There are 4 type of facets.
	// a     only vertex index.
	// a/b   vertex and texture index.
	// a/b/c vertex, texture and normal index.
	// a//c  vertex and normal index.
	else if(_token=="f")
	  {
	    // Read the remaining line for the facet.
	    std::string _line;
	    std::getline(_file, _line);
	    // Split the line into facet's vertices.
	    // The length of _vertices is equal to the number of vertices for this face.
	    std::istringstream _stream(_line);
	    std::vector<std::string> _vertices;
	    std::copy(std::istream_iterator<std::string>(_stream),
		      std::istream_iterator<std::string>(),
		      std::back_inserter(_vertices));
	    // For each vertex read only the first number, which is the vertex index.
	    B.begin_facet();
	    for(size_t i=0 ; i<_vertices.size() ; ++i)
	      {
		std::string::size_type _pos = _vertices[i].find('/', 0);
		std::string _indexStr = _vertices[i].substr(0, _pos);
		B.add_vertex_to_facet(stoi(_indexStr)-1); // -1 is because OBJ file uses 1 based index.
	      }
	    B.end_facet();
	  }
      }

    B.end_surface();
  }
};
