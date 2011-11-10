# A C++ library for the efficient simulation of large dynamic network models
The `largenet` library is a collection of C++ classes providing a framework for the 
simulation of large discrete [adaptive networks][1]. It provides data structures
for an in-memory representation of undirected networks, in which every node and link 
can have an integer-valued state.

Efficient access to (random) nodes and links as well as (random) nodes and links
with a given state value is provided. A limited number of graph-theoretical measures
is implemented, such as the (state-resolved) degree distributions, the clustering 
coefficient, the nearest-neighbor degree correlations, and the average shortest path
length.

The `largenet` library has been developed mostly by Gerd Zschaler <gzschaler@googlemail.com>
based on ideas of Thilo Gross <gross@physics.org>. Most of this work is licensed under the
Creative Commons CC-BY-NC license. See COPYING for details.

For installation instructions, see INSTALL. For examples how to use the library, see the 
`examples` directory.

The `largenet` library has been used for simulations of [evolutionary games on graphs][2],
adaptive network models of [swarming][3], fish [schooling][4], epidemiological models,
and opinion formation models. 

[1]: http://dx.doi.org/10.1098/rsif.2007.1229 "T. Gross and B. Blasius (2008), J. R. Soc. Interface, 5, 259"
[2]: http://dx.doi.org/10.1088/1367-2630/12/9/093015 "G. Zschaler et al. (2010), New J. Phys., 12, 093015"
[3]: http://dx.doi.org/10.1088/1367-2630/13/7/073022 "C. Huepe et al. (2011), New J. Phys., 13, 073022"
[4]: http://www.sciencemag.org "I. Couzin et al. (2011), to be published in Science" 