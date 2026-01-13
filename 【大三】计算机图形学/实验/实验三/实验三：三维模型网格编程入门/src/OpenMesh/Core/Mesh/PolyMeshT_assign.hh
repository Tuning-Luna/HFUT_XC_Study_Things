{
  This::clear();

  typename OtherMesh::ConstVertexIter  vIt(_rhs.vertices_begin()), 
                                       vEnd(_rhs.vertices_end());


  for (; vIt != vEnd; ++vIt)
    add_vertex(_rhs.point(vIt));


  typename OtherMesh::ConstFaceIter  fIt(_rhs.faces_begin()), 
                                     fEnd(_rhs.faces_end());
  std::vector<VertexHandle>          face_vhandles;

  for (; fIt != fEnd; ++fIt)
  {
    typename OtherMesh::ConstFaceVertexIter 
      fvIt=_rhs.cfv_iter(_rhs.handle(*fIt));
    
    face_vhandles.clear();

    for (; fvIt; ++fvIt)
      face_vhandles.push_back(fvIt.handle());

    add_face(face_vhandles);
  }

  return *this;
}
