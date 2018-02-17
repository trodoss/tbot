using System;
using System.Collections;
using System.IO;
using System.Xml;

//TMX Map File Reader
//based on TMX Map Format 1.1
//http://docs.mapeditor.org/en/latest/reference/tmx-map-format/

//Note: This is not a complete implementation -- this only facilitates
//what is being used in the map-pack utility.
namespace map_pack
{
    public class TMXFileReader
    {
		
		public static TMXTileset LoadTileset (string fileName) {
			return LoadTileset(null, fileName);
		}
		public static TMXTileset LoadTileset (TMXTileset tileset, string fileName) {
			if (tileset == null) tileset = new TMXTileset();
			using (XmlReader reader = XmlReader.Create(fileName)) {
				while (reader.Read()) {
					switch (reader.NodeType)
					{	
						case XmlNodeType.Element:
						    switch (reader.Name.ToLower()) 
                            {
                                case "tileset":
								tileset.Name = reader.GetAttribute("name");
		                        tileset.TileWidth = Int32.Parse(reader.GetAttribute("tilewidth"));
                                tileset.TileHeight = Int32.Parse(reader.GetAttribute("tileheight"));
								tileset.TileCount = Int32.Parse(reader.GetAttribute("tilecount"));
								tileset.Columns = Int32.Parse(reader.GetAttribute("columns"));
								break;

								case "image":
								break;		

								case "tile":
								TMXTile tile = new TMXTile();
								tile.Id = Int32.Parse(reader.GetAttribute("id"));
								if (reader.ReadToDescendant("properties"))
                                {
									if (reader.ReadToDescendant("property")) {
                                        do
                                        {
                                            TMXProperty prop = new TMXProperty();
                                            prop.Name = reader.GetAttribute("name");
                                            prop.Value = reader.GetAttribute("value");
                                            tile.Properties.Add(prop);
                                            prop = null;
        
                                        } while (reader.ReadToNextSibling("property"));
                                    }
								}
								tileset.Tiles.Add(tile);
								tile = null;
								break;
							}
						break;
						
						default:
						break;
					}
				}
			}
			return tileset;
		}
		
        public static TMXFile Load(string fileName) {
            TMXFile file = new TMXFile();
            using (XmlReader reader = XmlReader.Create(fileName)) {
                while (reader.Read()) {
                    switch (reader.NodeType)
                    {
                        case XmlNodeType.Element:
                            switch (reader.Name.ToLower()) {
                                case "map":
                                file.Map.TiledVersion = reader.GetAttribute("tiledversion");
                                switch (reader.GetAttribute("orientation")) {
                                    case "orthogonal":
                                    file.Map.Orientation = OrientationType.Orthogonal;
                                    break;

                                    case "isometric":
                                    file.Map.Orientation = OrientationType.Isometric;
                                    break;    

                                    case "staggered":
                                    file.Map.Orientation = OrientationType.Staggered;
                                    break;     

                                    case "hexagonal":
                                    file.Map.Orientation = OrientationType.Hexagonal;
                                    break;                                                                                                      

                                    default:
                                    file.Map.Orientation = OrientationType.Orthogonal;
                                    break;
                                }
                                switch (reader.GetAttribute("renderorder")) {
                                    case "right-down":
                                    file.Map.RenderOrder = RenderOrderType.RightDown;
                                    break;

                                    case "right-up":
                                    file.Map.RenderOrder = RenderOrderType.RightUp;
                                    break;

                                     case "left-down":
                                    file.Map.RenderOrder = RenderOrderType.LeftDown;
                                    break;

                                    case "left-up":
                                    file.Map.RenderOrder = RenderOrderType.LeftUp;
                                    break;                                   
                                }

                                file.Map.Width = Int32.Parse(reader.GetAttribute("width"));
                                file.Map.Height = Int32.Parse(reader.GetAttribute("height"));
                                file.Map.TileWidth = Int32.Parse(reader.GetAttribute("tilewidth"));
                                file.Map.TileHeight = Int32.Parse(reader.GetAttribute("tileheight"));
                                file.Map.NextObjectId = Int32.Parse(reader.GetAttribute("nextobjectid"));
                                break;

                                case "tileset":
                                TMXTileset set = new TMXTileset();
                                set.FirstGid = Int32.Parse(reader.GetAttribute("firstgid"));
                                set.Source = reader.GetAttribute("source");
								
								if (reader.GetAttribute("name") != null) set.Name = reader.GetAttribute("name");
		                        if (reader.GetAttribute("tilewidth") != null) set.TileWidth = Int32.Parse(reader.GetAttribute("tilewidth"));
                                if (reader.GetAttribute("tileheight") != null) set.TileHeight = Int32.Parse(reader.GetAttribute("tileheight"));
								if (reader.GetAttribute("tilecount") != null) set.TileCount = Int32.Parse(reader.GetAttribute("tilecount"));
								if (reader.GetAttribute("columns") != null) set.Columns = Int32.Parse(reader.GetAttribute("columns"));
								
								if (set.Source != null) {
									set = LoadTileset(set, set.Source);
								}
                                file.Map.Tilesets.Add(set);
                                set = null;
                                break;

                                case "layer":
                                TMXLayer layer = new TMXLayer();
                                layer.Name = reader.GetAttribute("name");
                                layer.Width = Int32.Parse(reader.GetAttribute("width"));
                                layer.Height = Int32.Parse(reader.GetAttribute("height"));
                                if (reader.ReadToDescendant("data"))
                                {
                                    TMXData dat = new TMXData();
                                    switch (reader.GetAttribute("encoding")) {
                                        case "base64":
                                        dat.Encoding = EncodingType.base64;
                                        break;

                                        case "csv":
                                        dat.Encoding = EncodingType.csv;
                                        break;
                                    }
                                    dat.Value = reader.ReadInnerXml();
                                    layer.Data = dat;
                                    dat = null;
                                }
                                file.Map.Layers.Add(layer);
                                break;

                            }
                            break;

                        case XmlNodeType.Text:
                            break;
                    }                   
                }
            }    
            return file;     
        }
    }
}