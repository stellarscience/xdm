/** \file
 * Main Doxygen documentation page. 
 */

/** \mainpage eXtensible Data Model (XDM) API Documentation

        <h3>Technical Reference</h3>

The eXtensible Data Model (XDM) library is an API for reading and writing mesh
based array data to disk. The API uses XDMF, an open source standard definition
detailed at http://www.xdmf.org for mesh data as its native exchange format. The
format defines a standard way of writing computational results associated with
an analysis over a grid to a format that can then be used for visualization or
as a standard data hub for exchange between applications.

The API and standard format are defined independent of any particular
application domain. There are a broad array of application models that fit the
underlying data model. The API is divided into components, each with a limited
scope and well defined interface:

- xdm: Core object model and general interface definitions.
- xdmGrid: Classes for defining structured and unstructured grids and
defining attribute values over them.
- xdmHdf: HDF5 dataset implementation and related utilities.
- xdmComm: A dataset implementation for writing data in a parallel
environment and related utilities.

Each component defines a subset of functionality that is implemented using the
core data model defined in the xdm library. While specializing interfaces in the
core library, each component is expected to be generic enough not to be attached
to a particular application domain.

In addition to the core libraries, there are plugins that implement the core
interfaces for reading formats. Currently documented plugins are:

- xdmf: Plugin for reading and writing data in the XDMF file format.

The xdmIntegrationTest project serves as a simple example of writing the output
of a simple sine function over a grid and acts as a test suite to integrate
multiple XDM components into working code.

*/

