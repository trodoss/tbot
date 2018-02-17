using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace map_pack
{
    //TMX Map File objects
    public enum OrientationType {
        Orthogonal,
        Isometric,
        Staggered,
        Hexagonal
    }
    
    public enum RenderOrderType {
        RightDown,
        RightUp,
        LeftDown,
        LeftUp
    }

    public enum EncodingType {
        base64,
        csv
    }
    public class TMXData {
        public EncodingType Encoding {get; set;}
        public string Value {get; set;}
    }
    public class TMXTileset {
		public TMXTileset() {
			this.Tiles = new List<TMXTile>();
		}
        public int FirstGid { get; set;}
        public string Source {get; set;}   
		public string Name {get; set;}
		public int TileWidth {get; set;}
		public int TileHeight {get; set;}
		public int TileCount {get; set;}
		public int Columns {get; set;}
		
		public List<TMXTile>Tiles {get; set;}
    }
	
	public class TMXTile {
		public TMXTile() {
			this.Properties = new List<TMXProperty>();
		}
		public int Id {get; set;}
		public List<TMXProperty>Properties { get; set;}
		
	}
	
	public class TMXProperty {
		public string Name {get; set;}
		public string Value {get; set;}
		
	}
    public class TMXLayer {
        public TMXLayer() {
            this.Data = new TMXData();
        }
        public string Name {get; set;}
        public int Width {get; set;}
        public int Height {get; set;}      
        public TMXData Data {get; set;} 
    }

   public class TMXMap {
       public TMXMap () {
           this.Layers = new List<TMXLayer>();
           this.Tilesets = new List<TMXTileset>();
       }
       public string Version {get; set;}
       public string TiledVersion {get; set;}
       public OrientationType Orientation {get; set;}
       public RenderOrderType RenderOrder { get; set;}
       public int Width {get; set;}
       public int Height {get; set;}
       public int TileWidth {get; set;}
       public int TileHeight {get; set;}
       public int NextObjectId {get; set;}
       public List<TMXTileset>  Tilesets {get; set;}
       public List<TMXLayer> Layers {get; set;}
   }

   public class TMXFile
    {
        public TMXFile() {
            this.Map = new TMXMap();
        }
        public TMXMap Map {get; set;}

        public Map ToMap(string layerName) {
            TMXLayer thisLayer = this.Map.Layers.FirstOrDefault(s => s.Name == layerName);
            if (thisLayer == null) throw new Exception ("Unable to find layer '"+ layerName + "'");

            Map newMap = new Map();
            newMap.Rows = this.Map.Width;
			newMap.Columns = this.Map.Height;
            newMap.Cells = ParseTMXData(thisLayer.Data);
            return newMap;
        }

        private MapCellList ParseTMXData(TMXData dat) {
            MapCellList list = new MapCellList();

            string[] values = dat.Value.Split(',').Select(sValue => sValue.Trim()).ToArray();
            foreach (string val in values) {
                list.Add(new MapCell(Int32.Parse(val)));
            }
            return list;
        }

        private TMXTile GetTileById( int id) {
            foreach (TMXTileset set in this.Map.Tilesets) {
                TMXTile found = set.Tiles.FirstOrDefault(t => (set.FirstGid + t.Id) == id);
                if (found != null) return found;
            }          
            return null;
        }

        public ElementList ToElementList(string layerName) {
            TMXLayer thisLayer = this.Map.Layers.FirstOrDefault(s => s.Name == layerName);
            if (thisLayer == null) throw new Exception ("Unable to find layer '"+ layerName + "'");

            ElementList newElementList = new ElementList();
            MapCellList list = ParseTMXData(thisLayer.Data);

            int tileX = 0;
            int tileY = 0;
            foreach (MapCell cell in list) {
                if (cell.TileID > 0) {
                    TMXTile tile = GetTileById(cell.TileID);
                    string type = tile.Properties.FirstOrDefault(p => p.Name.ToLower() == "type").Value;
                    if (type == null) type = "0";
                    string state = tile.Properties.FirstOrDefault(p => p.Name.ToLower() == "state").Value;
                    if (state == null) state = "0";

                    Element ele = new Element() 
                    { 
                        Type = type, 
                        State = state, 
                        X = (tileX * this.Map.TileWidth), 
                        Y = (tileY * this.Map.TileHeight)
                    };
                    newElementList.Add(ele);
                }

                tileX++;
                if (tileX > this.Map.Width) {
                    tileX = 0;
                    tileY++;
                }

            }

            return newElementList;
        }
    }
}