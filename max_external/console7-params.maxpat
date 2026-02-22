{
  "patcher": {
    "fileversion": 1,
    "appversion": {
      "major": 9,
      "minor": 0,
      "revision": 0,
      "architecture": "x64"
    },
    "classnamespace": "box",
    "rect": [34.0, 87.0, 816.0, 468.0],
    "boxes": [
      {
        "box": {
          "id": "obj-1",
          "maxclass": "newobj",
          "patching_rect": [38.0, 26.0, 96.0, 22.0],
          "text": "airfx.console7~"
        }
      },
      {
        "box": {
          "id": "obj-2",
          "maxclass": "message",
          "patching_rect": [38.0, 63.0, 38.0, 22.0],
          "text": "getattr"
        }
      },
      {
        "box": {
          "id": "obj-3",
          "maxclass": "message",
          "patching_rect": [83.0, 63.0, 70.0, 22.0],
          "text": "getattr fader"
        }
      },
      {
        "box": {
          "id": "obj-4",
          "maxclass": "message",
          "patching_rect": [160.0, 63.0, 71.0, 22.0],
          "text": "fader 0.772"
        }
      },
      {
        "box": {
          "id": "obj-5",
          "maxclass": "flonum",
          "patching_rect": [238.0, 63.0, 66.0, 22.0],
          "minimum": 0.0,
          "maximum": 1.0
        }
      },
      {
        "box": {
          "id": "obj-6",
          "maxclass": "newobj",
          "patching_rect": [38.0, 104.0, 69.0, 22.0],
          "text": "print attrs"
        }
      },
      {
        "box": {
          "id": "obj-7",
          "maxclass": "newobj",
          "patching_rect": [124.0, 104.0, 78.0, 22.0],
          "text": "attrui @attr fader"
        }
      }
    ],
    "lines": [
      {
        "patchline": {
          "source": ["obj-2", 0],
          "destination": ["obj-1", 0]
        }
      },
      {
        "patchline": {
          "source": ["obj-3", 0],
          "destination": ["obj-1", 0]
        }
      },
      {
        "patchline": {
          "source": ["obj-4", 0],
          "destination": ["obj-1", 0]
        }
      },
      {
        "patchline": {
          "source": ["obj-5", 0],
          "destination": ["obj-1", 0]
        }
      },
      {
        "patchline": {
          "source": ["obj-1", 0],
          "destination": ["obj-6", 0]
        }
      },
      {
        "patchline": {
          "source": ["obj-7", 0],
          "destination": ["obj-1", 0]
        }
      },
      {
        "patchline": {
          "source": ["obj-1", 2],
          "destination": ["obj-7", 0]
        }
      }
    ]
  }
}
