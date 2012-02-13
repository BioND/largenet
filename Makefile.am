ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = largenet-@PACKAGE_VERSION@.pc

EXTRA_DIST = largenet-@PACKAGE_VERSION@.pc README.md Doxyfile

lib_LTLIBRARIES = liblargenet-@PACKAGE_VERSION@.la
liblargenet_@PACKAGE_VERSION@_la_LDFLAGS = -version-info 0:0:0
liblargenet_@PACKAGE_VERSION@_la_CPPFLAGS = $(BOOST_CPPFLAGS)

liblargenet_@PACKAGE_VERSION@_la_SOURCES = \
		largenet/measures/correlations.cpp \
		largenet/io/NodeEdgelistOut.cpp \
		largenet/io/EdgelistOut.cpp \
		largenet/io/EdgelistIn.cpp \
		largenet/io/NetIn.cpp \
		largenet/io/NetOut.cpp \
		largenet/io/NodeEdgelistIn.cpp \
		largenet/TripleMultiNetwork.cpp \
		largenet/Network.cpp \
		largenet/TripleNetwork.cpp \
		largenet/base/BasicNetwork.cpp \
		largenet/base/repo/repo_base.cpp \
		largenet/myrng1.2/myrngWELL.cpp \
		largenet/motifs/QuadLineMotif.cpp \
		largenet/motifs/TripleMotif.cpp \
		largenet/motifs/detail/motif_construction.cpp \
		largenet/motifs/LinkMotif.cpp \
		largenet/motifs/NodeMotif.cpp \
		largenet/motifs/QuadStarMotif.cpp \
		largenet/generators.cpp 
		
nobase_include_HEADERS = \
		largenet/myrng1.2/myrngWELL.h \
		largenet/measures/path.h \
		largenet/measures/correlations.h \
		largenet/measures/measures.h \
		largenet/measures/component.h \
		largenet/io/NetworkIO.h \
		largenet/io/NodeEdgelistIn.h \
		largenet/io/EdgelistOut.h \
		largenet/io/NodeEdgelistOut.h \
		largenet/io/NetOut.h \
		largenet/io/iodefs.h \
		largenet/io/NetIn.h \
		largenet/io/EdgelistIn.h \
		largenet/base/BasicNetwork.h \
		largenet/TripleMultiNetwork.h \
		largenet/base/TLink.h \
		largenet/base/types.h \
		largenet/base/Triple.h \
		largenet/Network.h \
		largenet/base/traits.h \
		largenet/MultiNetwork.h \
		largenet/base/TypedNetwork.h \
		largenet/TripleNetwork.h \
		largenet/base/Node.h \
		largenet/base/state_calculators.h \
		largenet/base/Link.h \
		largenet/motifs/QuadLineMotif.h \
		largenet/motifs/TripleMotif.h \
		largenet/motifs/LinkMotif.h \
		largenet/motifs/NodeMotif.h \
		largenet/motifs/detail/motif_construction.h \
		largenet/motifs/QuadStarMotif.h \
		largenet/motifs/MotifSet.h \
		largenet/motifs.h \
		largenet/generators.h \
		largenet/tnt/tnt_array2d.h \
		largenet/base/repo/iterators.h \
		largenet/base/repo/CategorizedRepository.h \
		largenet/base/repo/types.h \
		largenet/base/repo/repo_base.h \
		largenet/tnt/tnt.h \
		largenet/tnt/tnt_array1d.h \
		largenet/tnt/tnt_subscript.h \
		largenet/tnt/tnt_fortran_array2d_utils.h \
		largenet/tnt/tnt_fortran_array3d.h \
		largenet/tnt/tnt_i_refvec.h \
		largenet/tnt/tnt_fortran_array3d_utils.h \
		largenet/tnt/tnt_stopwatch.h \
		largenet/tnt/tnt_array2d_utils.h \
		largenet/tnt/tnt_sparse_matrix_csr.h \
		largenet/tnt/jama_eig.h \
		largenet/tnt/tnt_array3d_utils.h \
		largenet/tnt/jama_cholesky.h \
		largenet/tnt/tnt_fortran_array2d.h \
		largenet/tnt/tnt_version.h \
		largenet/tnt/tnt_fortran_array1d_utils.h \
		largenet/tnt/tnt_vec.h \
		largenet/tnt/jama_lu.h \
		largenet/tnt/tnt_array1d_utils.h \
		largenet/tnt/jama_svd.h \
		largenet/tnt/tnt_math_utils.h \
		largenet/tnt/jama_qr.h \
		largenet/tnt/tnt_array3d.h \
		largenet/tnt/tnt_fortran_array1d.h \
		largenet/tnt/tnt_cmat.h \
		largenet.h

if BUILD_EXAMPLES
noinst_PROGRAMS = votermodel swarm
votermodel_LDADD = liblargenet-@PACKAGE_VERSION@.la 
votermodel_SOURCES = \
		examples/votermodel/vm.cpp \
		examples/votermodel/VoterModel.cpp \
		examples/votermodel/VoterModel.h 
swarm_LDADD = liblargenet-@PACKAGE_VERSION@.la
swarm_CXXFLAGS = $(BOOST_CXXFLAGS)
swarm_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIBS)
swarm_LIBS = $(BOOST_PROGRAM_OPTIONS_LIBS)
 
swarm_SOURCES = \
		examples/swarm/Options.cpp \
		examples/swarm/swarm.cpp \
		examples/swarm/SwarmApp.cpp \
		examples/swarm/SwarmModel.cpp \
		examples/swarm/Options.h \
		examples/swarm/StateAlignEvent.h \
		examples/swarm/StateSwitchEvent.h \
		examples/swarm/SwarmApp.h \
		examples/swarm/SwarmModel.h \
		examples/swarm/output/DegDistOutput.cpp \
		examples/swarm/output/DiffDegDistOutput.cpp \
		examples/swarm/output/IntervalOutput.cpp \
		examples/swarm/output/NodeStateOutput.cpp \
		examples/swarm/output/Outputter.cpp \
		examples/swarm/output/StateIntervalOutput.cpp \
		examples/swarm/output/SuggestionPower.cpp \
		examples/swarm/output/TimeSeriesOutput.cpp \
		examples/swarm/output/DegDistOutput.h \
		examples/swarm/output/DiffDegDistOutput.h \
		examples/swarm/output/IntervalOutput.h \
		examples/swarm/output/NodeStateOutput.h \
		examples/swarm/output/Outputter.h \
		examples/swarm/output/StateIntervalOutput.h \
		examples/swarm/output/SuggestionPower.h \
		examples/swarm/output/TimeSeriesOutput.h
endif

if HAVE_DOXYGEN

DOCDIR = doc
MOSTLYCLEANFILES = -r $(DOCDIR)
	
doxygen-doc: $(DOCDIR)/doxygen.stamp 

$(DOCDIR)/doxygen.stamp: Doxyfile
	$(DOXYGEN) $(DOXYFLAGS) $<
	echo Timestamp > $@

endif

