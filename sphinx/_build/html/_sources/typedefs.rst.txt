C++ Typedefs
------------

The CGAL library is a templates library designed with a high level of modularity in mind. As CGAL
also integrates the template library` boost <https://www.boost.org/>`_ it can become a hassle specifying complex
types, for example,

.. code-block:: cpp

   boost::iterator_property_map<IteratorVectorVertexindex_3, VertexIdPropertyMap_3> object = some_code_here


Thus, to reduce the need for long winded, complex typenames, templated types have been aliased with the
typdef specifier. Templated types can be included via,

.. code-block:: cpp

   #include <CGALMethods/types.hpp>


The typdefs have been styled such that individual typenames can be delineated by camel case, for example,

.. code-block:: cpp

    typedef std::vector<std::string> VectorString;


VectorString is the typename for a vector of strings.

When using `std::pair <https://en.cppreference.com/w/cpp/utility/pair>`_, if the pairs are of the same type, the typedef is eluded

.. code-block:: cpp

   typedef std::pair<VectorString, VectorString> PairVectorString;


this is the same case for unordered maps where the key is of the same type as the value.

.. code-block:: cpp

   typedef boost::unordered_map<Vertexindex_3, Vertexindex_3> UmapVertexindex_3;


PairVectorString is a pair of vectors of strings.

Bellow we list the typedefs defined in the types namespace of the CGALMethods namespace,

.. code-block:: cpp

   // vectors, maps, pairs of strings
   typedef std::vector<std::string> VectorString;
   typedef std::pair<VectorString, VectorString> PairVectorString;
   typedef std::pair<std::string, std::string> PairString;
   typedef std::vector<PairString> VectorPairString;
   typedef std::map<std::string, std::string> MapStringString;

   // vectors primitive types
   typedef std::vector<double> VectorDouble;
   typedef std::vector<int> VectorInt;

   // iterators primitve types
   typedef VectorDouble::iterator IteratorDouble;
   typedef VectorInt::iterator IteratorInt;

   // if data types are to be modified change this Kernel typedef
   typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

   // Vector kernel data structure
   typedef Kernel::Vector_3 Vector_3;
   typedef Kernel::Vector_2 Vector_2;

   // point definitions
   typedef CGAL::Point_2<Kernel> Point_2;
   typedef CGAL::Point_3<Kernel> Point_3;

   // mesh definitions
   typedef CGAL::Surface_mesh<Point_2> Mesh_2;
   typedef CGAL::Surface_mesh<Point_3> Mesh_3;

   // Mesh properties metadata
   typedef std::pair<Mesh_2, VectorPairString> PairMeshVectorPairString_2;
   typedef std::pair<Mesh_3, VectorPairString> PairMeshVectorPairString_3;

   // Mesh datastructures
   typedef typename Mesh_3::Vertex_index Vertexindex_3;
   typedef typename Mesh_3::Halfedge_index Halfedgeindex_3;
   typedef typename Mesh_3::Edge_index Edgeindex_3;
   typedef typename Mesh_3::Face_index Faceindex_3;

   typedef typename Mesh_2::Vertex_index Vertexindex_2;
   typedef typename Mesh_2::Halfedge_index Haledgeindex_2;
   typedef typename Mesh_2::Edge_index Edgeindex_2;
   typedef typename Mesh_2::Face_index Faceindex_2;

   // Pair of mesh datastructures
   typedef typename std::pair<Vertexindex_3, Vertexindex_3>
         PairVertexindex_3;
   typedef typename std::pair<Vertexindex_3, Vertexindex_3>
         PairVertexindex_2;
   typedef typename std::pair<Vertexindex_3, Vertexindex_3>
         PairVertexindex_2;
   typedef std::pair<Point_3,Vector_3> PairPointVector;
   typedef std::pair<Halfedgeindex_3, int> PairHalfedgeindexInt_3;

   // boost mesh structures
   typedef typename boost::vertex_index_t TypeVertexindex;

   // unordered propety maps
   typedef boost::unordered_map<Vertexindex_3, double>
         UmapVertexindexDouble_3;
   typedef boost::unordered_map<Vertexindex_3, Vertexindex_3>
         UmapVertexindex_3;

   // vectors of mesh datastructures
   typedef std::vector<Vertexindex_3> VectorVertexindex_3;
   typedef std::vector<Halfedgeindex_3> VectorHalfedgeindex_3;
   typedef std::vector<Point_3> VectorPoint_3;

   // iterators of mesh datastructures
   typedef VectorVertexindex_3::iterator IteratorVectorVertexindex_3;
   typedef VectorDouble::iterator IteratorVectorDouble_3;

   // boost property maps
   typedef typename boost::property_map<Mesh_3, TypeVertexindex>::type VertexIdPropertyMap_3;

   // boost iterator maps
   typedef typename boost::iterator_property_map
         <IteratorVectorVertexindex_3, VertexIdPropertyMap_3> IteratorPmapVertexindex_3;
   typedef typename boost::iterator_property_map
         <IteratorVectorDouble_3, VertexIdPropertyMap_3> IteratorPmapdouble_3;

   // boost unordered maps
   typedef boost::unordered_map<Vertexindex_3, double> UmapVertexindexDouble_3;
   typedef boost::unordered_map<Vertexindex_3, Vertexindex_3> UmapVertexindex_3;
   typedef boost::unordered_map<Halfedgeindex_3, bool> UmapHalfedgeindexBool_3;

   typedef boost::unordered_map<Halfedgeindex_3, Vertexindex_3> UmapHalfedgeindexVertexindex_3;
   // pair umaps
   typedef std::pair<UmapVertexindexDouble_3, UmapVertexindex_3> PairUmapdoubleVertexindex_3;
   typedef std::pair<UmapVertexindexDouble_3, UmapVertexindex_3>
         PairUmapDoubleUmapVertexindex_3;

   // mesh property_maps
   typedef typename Mesh_3::Property_map<Vertexindex_3, double>
         MapVertexindexDouble_3;
   typedef typename Mesh_3::Property_map<Vertexindex_3, Vertexindex_3>
         MapVertexindex_3;
   typedef typename Mesh_3::Property_map<Vertexindex_3, Vector_3>
         MapVertexindexVector_3;
   typedef Mesh_3::Property_map<Vertexindex_3, Point_2>
         MapVertexindex_3_to_Point2;
   typedef Mesh_3::Property_map<Vertexindex_3, Point_3>
         MapVertexindex_3_to_Point3;

   // output tuples
   typedef std::tuple<Vertexindex_3, Vertexindex_3, UmapVertexindex_3>
         TupleVertexVertexUmapvertex_3;

   // Unique hash maps
   typedef CGAL::Unique_hash_map<Halfedgeindex_3, Point_2> UHMHalfedgeindexPoint_32; //32 means map from Mesh_3 to Mesh_2
   typedef CGAL::Unique_hash_map<Halfedgeindex_3, Point_3> UHMHalfedgeindexPoint_3;
   typedef CGAL::Unique_hash_map<Vertexindex_3, Point_3> UHMVertexindexPoint_3;
   typedef CGAL::Unique_hash_map<Edgeindex_3, bool> UHMEdgeindexBool_3;
   typedef CGAL::Unique_hash_map<Vertexindex_3, bool> UHMVertexindexBool_3;

   // nested vectors
   typedef std::vector<std::vector<Halfedgeindex_3>> NestedVectorHalfedgeindex_3;
   typedef std::vector<std::vector<Vertexindex_3>> NestedVectorVertexindex_3;
   typedef std::vector<std::vector<Edgeindex_3>> NestedVectorEdgeindex_3;

   // bellow shows the necessities for a seam mesh----------------------------------

   // associative propeerty maps
   typedef boost::associative_property_map<UHMHalfedgeindexPoint_32> APMHalfedgeindexPoint_32; // <--- required for the seam mesh
   typedef boost::associative_property_map<UHMEdgeindexBool_3> APMEdgeindexBool_3;
   typedef boost::associative_property_map<UHMVertexindexBool_3> APMVertexindexBool_3;

   typedef CGAL::Seam_mesh<Mesh_3, APMEdgeindexBool_3, APMVertexindexBool_3> SeamMesh;


To change the kernel we can modify the line,

.. code-block:: cpp

   typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;