AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

lib_LTLIBRARIES = liblargenet-@LARGENET_API_VERSION@.la
liblargenet_@LARGENET_API_VERSION@_la_SOURCES = \
		measures/correlations.cpp \
		io/NodeEdgelistOut.cpp \
		io/EdgelistOut.cpp \
		io/EdgelistIn.cpp \
		io/NetIn.cpp \
		io/NetOut.cpp \
		io/NodeEdgelistIn.cpp \
		network/TripleMultiNetwork.cpp \
		network/Network.cpp \
		network/TripleNetwork.cpp \
		network/repo/repo_base.cpp \ \
		network/BasicNetwork.cpp \
		myrng1.2/myrngWELL.cpp \
		boost/boost_largenet_multigraph.cpp \
		motifs/QuadLineMotif.cpp \
		motifs/TripleMotif.cpp \
		motifs/detail/motif_construction.cpp \
		motifs/LinkMotif.cpp \
		motifs/NodeMotif.cpp \
		motifs/QuadStarMotif.cpp \
		generators.cpp \
		network/repo/iterators.h \
		network/repo/CategorizedRepository.h \
		network/repo/types.h \
		network/repo/repo_base.h \
		myrng1.2/myrngWELL.h \
		tnt/tnt.h \
		tnt/tnt_array1d.h \
		tnt/tnt_subscript.h \
		tnt/tnt_fortran_array2d_utils.h \
		tnt/tnt_fortran_array3d.h \
		tnt/tnt_array2d.h \
		tnt/tnt_i_refvec.h \
		tnt/tnt_fortran_array3d_utils.h \
		tnt/tnt_stopwatch.h \
		tnt/tnt_array2d_utils.h \
		tnt/tnt_sparse_matrix_csr.h \
		tnt/jama_eig.h \
		tnt/tnt_array3d_utils.h \
		tnt/jama_cholesky.h \
		tnt/tnt_fortran_array2d.h \
		tnt/tnt_version.h \
		tnt/tnt_fortran_array1d_utils.h \
		tnt/tnt_vec.h \
		tnt/jama_lu.h \
		tnt/tnt_array1d_utils.h \
		tnt/jama_svd.h \
		tnt/tnt_math_utils.h \
		tnt/jama_qr.h \
		tnt/tnt_array3d.h \
		tnt/tnt_fortran_array1d.h \
		tnt/tnt_cmat.h 

nobase_include_HEADERS = \
		measures/path.h \
		measures/correlations.h \
		measures/measures.h \
		measures/component.h \
		io/NetworkIO.h \
		io/NodeEdgelistIn.h \
		io/EdgelistOut.h \
		io/NodeEdgelistOut.h \
		io/NetOut.h \
		io/iodefs.h \
		io/NetIn.h \
		io/EdgelistIn.h \
		network/BasicNetwork.h \
		network/TripleMultiNetwork.h \
		network/TLink.h \
		network/types.h \
		network/Triple.h \
		network/Network.h \
		network/traits.h \
		network/MultiNetwork.h \
		network/TypedNetwork.h \
		network/TripleNetwork.h \
		network/Node.h \
		network/state_calculators.h \
		network/Link.h \
		boost/boost_largenet_multigraph.h \
		motifs/QuadLineMotif.h \
		motifs/TripleMotif.h \
		motifs/LinkMotif.h \
		motifs/NodeMotif.h \
		motifs/detail/motif_construction.h \
		motifs/QuadStarMotif.h \
		motifs/MotifSet.h \
		largenet.h \
		motifs.h \
		generators.h

liblargenet_@LARGENET_API_VERSION@_la_LDFLAGS = -version-info $(LARGENET_SO_VERSION)
library_includedir = $(includedir)/largenet
