# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'TCP Server'
copyright = '2022, Iliya Iliev'
author = 'Iliya Iliev'

# -- Constants ---------------------------------------------------------------

VERSION_FILE = '../../include/tcp_server_library.h'

# -- Helpers -----------------------------------------------------------------
def __get_versh(tags, version_file=VERSION_FILE):
    version = "v"

    with open(version_file) as f:
        version_file = f.readlines()

    for tag in tags:
        for line in version_file:
            if tag in line:
                # Return only the versh number
                versh = line.strip("#define " + tag).rstrip()
                version += versh  + '.'
                break

    return version

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.extlinks",
    "sphinx.ext.intersphinx",
    "sphinx.ext.mathjax",
    "sphinx.ext.todo",
    #"sphinx.ext.viewcode",
    'sphinx_rtd_theme',
    'sphinxcontrib.autoprogram',
    "myst_parser",
    "sphinx_copybutton",
    "sphinx_design",
    "sphinx_inline_tabs",
    'linuxdoc.cdomain',        # Replacement for the sphinx c-domain.
    'sphinx_c_autodoc',
    'sphinx_c_autodoc.napoleon',
    'sphinx_c_autodoc.viewcode',
    'sphinx.ext.todo',
    'sphinx.ext.coverage',
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []

# -- API C files -------------------------------------------------------------
c_autodoc_roots = ['../../src',
                   '../../include']

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = "sphinx_rtd_theme"

autosectionlabel_prefix_document = True
html_theme_options = {
    'logo_only': False,
    'display_version': True,
    'prev_next_buttons_location': 'bottom',
    'style_external_links': True,
    'vcs_pageview_mode': '',
    'collapse_navigation': True,
    'sticky_navigation': True,
    'navigation_depth': 2,
    'includehidden': True,
    'titles_only': True,
    'sticky_navigation': True
}

version = __get_versh(['TCP_SERVER_VERSION_MAJOR',
                       'TCP_SERVER_VERSION_MINOR',
                       'TCP_SERVER_VERSION_PATCH'])

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_show_sourcelink = False
html_static_path = ['_static']
html_title = "TCP Server"
