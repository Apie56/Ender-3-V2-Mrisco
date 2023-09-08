/**
 * DWIN G-code thumbnail preview
 * Author: Miguel A. Risco-Castillo
<<<<<<< HEAD
 * version: 3.5.3
 * Date: 2023/04/25
=======
 * version: 4.2.3
 * Date: 2023/08/15
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
 * For commercial applications additional licenses can be requested
 */

#pragma once

<<<<<<< HEAD
void Preview_DrawFromSD();
void Preview_Invalidate();
bool Preview_Valid();
void Preview_Show();
=======
class GPreview {
  public:
    static bool isValid();
    static void invalidate();
    static void draw();
    static void show();
};

extern GPreview gPreview;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
