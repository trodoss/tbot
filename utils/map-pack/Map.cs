using System;
using System.Collections;
using System.Collections.Generic;

namespace map_pack {
    public class MapCell {					
		public int TileID;
		public int Attributes;
		
		public MapCell() {
		}

		public MapCell(int tileID) {
			this.TileID = tileID;
		}		

		public MapCell(int tileID, int attributes) {
			this.TileID = tileID;
			this.Attributes = attributes;
		}		
	}
    public class MapPattern {
		public MapCellList Cells;
		public int Count;
		
		public MapPattern() {
			this.Cells = new MapCellList();
			this.Count = 1;
		}
	}
    public class MapPatternList : List<MapPattern> { }

    public class MapCellList : List<MapCell> { }

	public class Map {					
		public int Rows;
		public int Columns;
		public MapCellList Cells;
		public MapPatternList Patterns;
		public ReductionType Reduction;

		public bool IncludeDimensions;
		
		public enum ReductionType {None, RLE, Pattern}
		
		public Map() {
			this.Cells = new MapCellList();
			this.Reduction = ReductionType.None;
            this.Patterns = new MapPatternList();
		}
		
		public Map(int rows, int columns) {
			this.Rows = rows;
			this.Columns = columns;
			this.Cells = new MapCellList();
			this.Patterns = new MapPatternList();
			this.IncludeDimensions = true;
			this.Reduction = ReductionType.None;
		}
	}  
}