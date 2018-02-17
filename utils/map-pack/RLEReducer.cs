using System;

namespace map_pack {
	public class RLEReducer {
		public static Map Reduce (Map map) {	
			Map reducedMap = map;
			
			try {	
				for (int i=0; i< map.Cells.Count; i++) {
					int runLength = 1;
					while ((i + 1 < map.Cells.Count) && (map.Cells[i].TileID == map.Cells[i+1].TileID)) {
						runLength++;
						i++;
					}
					//generate the map pattern for the 'run'
					MapPattern thisPattern = new MapPattern();
					thisPattern.Cells.Add(new MapCell(map.Cells[i].TileID));
					thisPattern.Count = runLength;
					reducedMap.Patterns.Add(thisPattern);
					thisPattern = null;
				}
										
			} catch (Exception e){
				throw new Exception("Error in reducing map data using RLE (Run-Length Encoding) method - " + e.ToString());
			}

			return reducedMap;
		}
	
	}
}