# import to process args
import sys
import os
import glob

try:
    import argparse
except ImportError:
    # since  Python 2.6 and earlier don't have argparse, we simply provide
    # the source for the same as _argparse and we use it instead.
    import _argparse as argparse

# import annotations
from autobahn.wamp import exportRpc

# import paraview modules.
from paraview import simple, web, servermanager, web_helper

# ==============================================

class ToggleFile(web.ParaViewServerProtocol):

    fileList = []
    proxyList = {}

    @exportRpc("listFiles")
    def listFiles(self):
        return ToggleFile.fileList

    @exportRpc("toggle")
    def toggle(self, name, visibility):
        if visibility:
            simple.Show(ToggleFile.proxyList[name])
        else:
            simple.Hide(ToggleFile.proxyList[name])

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="ParaView/Web Toggle web-application")
    web.add_arguments(parser)
    parser.add_argument("--data-dir", default=os.getcwd(), help="path to data directory to list", dest="path")
    args = parser.parse_args()

    # Create file list
    ToggleFile.fileList = glob.glob(args.path + "/*.wrl")

    # Create proxy and representation for each file
    for file in ToggleFile.fileList:
        proxy = simple.OpenDataFile(file)
        ToggleFile.proxyList[file] = proxy
        rep = simple.Show(proxy)
        rep.DiffuseColor = [0.5, 0.1, 0.9]

    simple.Render()

    web.start_webserver(options=args, protocol=ToggleFile)
