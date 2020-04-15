"""
Theme for documentation projects related to CoR-Lab software.

CoR-Lab Homepage: `https://www.cor-lab.org/`_

.. codeauthor:: Johannes Wienke <jwienke@techfak.uni-bielefeld.de>
"""
import os

VERSION = (0, 1, 4)

__version__ = '.'.join(str(v) for v in VERSION)
__version_full__ = __version__


def get_theme_dir():
    """
    Returns path to directory containing this package's theme.

    This is designed to be used when setting the ``html_theme_path``
    option within Sphinx's ``conf.py`` file.
    """
    return os.path.abspath(os.path.dirname(os.path.dirname(__file__)))


def default_sidebars():
    """
    Returns a dictionary mapping for the templates used to render the
    sidebar on the index page and sub-pages.
    """
    return {
        '**': ['globaltoc.html',
               'links.html',
               'sourcelink.html',
               'searchbox.html']
    }
