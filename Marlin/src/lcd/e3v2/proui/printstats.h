/**
 * Print Stats page for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
<<<<<<< HEAD
 * Version: 1.4.0
 * Date: 2022/12/03
=======
 * Version: 2.1.0
 * Date: 2023/07/12
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
class PrintStatsClass {
public:
  static void Draw();
  static void Reset();
};

extern PrintStatsClass PrintStats;

void Goto_PrintStats();
void PrintStatsReset();
=======
class PrintStats {
public:
  static void draw();
  static void reset();
};

extern PrintStats printStats;

void gotoPrintStats();
void printStatsReset();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
