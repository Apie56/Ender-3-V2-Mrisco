/**
 * Mesh Viewer for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
<<<<<<< HEAD
 * version: 4.2.1
 * Date: 2023/05/05
=======
 * version: 6.1.1
 * Date: 2023/08/20
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

<<<<<<< HEAD
class MeshViewerClass {
public:
  static float max, min;
  static void DrawMeshGrid(const uint8_t csizex, const uint8_t csizey);
  static void DrawMeshPoint(const uint8_t x, const uint8_t y, const float z);
  static void Draw(bool withsave = false, bool redraw = true);
  static void DrawMesh(bed_mesh_t zval, const uint8_t csizex, const uint8_t csizey);
};

extern MeshViewerClass MeshViewer;

void Goto_MeshViewer(bool redraw);
=======
#include "../../../inc/MarlinConfig.h"

#define USE_GRID_MESHVIEWER

#if ENABLED(PROUI_EX)
  class MeshViewerPro {
  public:
    static float max, min;
    static uint8_t meshfont;
    static bool meshmode;
    static void drawBackground(const uint8_t csizex, const uint8_t csizey);
    static void drawPoint(const uint8_t x, const uint8_t y, const float z);
    static void drawMesh(const bed_mesh_t zval, const uint8_t csizex, const uint8_t csizey);
    static void drawViewer(const bool withsave=false, const bool redraw=true);
  };
  extern MeshViewerPro meshViewer;
#else
  class MeshViewer {
  public:
    static float max, min;
    static void drawBackground(const uint8_t csizex, const uint8_t csizey);
    static void drawPoint(const uint8_t x, const uint8_t y, const float z);
    static void drawMesh(const bed_mesh_t zval, const uint8_t csizex, const uint8_t csizey);
    static void drawViewer(const bool withsave=false, const bool redraw=true);
  };
  extern MeshViewer meshViewer;
#endif

void gotoMeshViewer(const bool redraw);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
