using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;

namespace map_pack {
    public class MapDataEntry {
        public int Length { get; set;}
        public string Text { get; set;}
    }
    public class MapDataWriter {
        private List<MapDataEntry> entries;

        public MapDataWriter() {
            this.entries = new List<MapDataEntry>();
        }

        public void AddMap(Map addMap) {
            MapDataEntry entry = new MapDataEntry();
            if (addMap.IncludeDimensions) {
                entry.Text += "// width, height," + Environment.NewLine;              
                entry.Text += addMap.Rows+ ","+addMap.Columns + "," + Environment.NewLine;
                entry.Length += 2;
            }

            switch (addMap.Reduction) {
					case Map.ReductionType.RLE:
						foreach (MapPattern thisPattern in addMap.Patterns) {
							if (thisPattern.Count > 1) {
                                int patternID = (thisPattern.Cells[0].TileID + 128);
                                int patternCount = thisPattern.Count;
                                if (thisPattern.Count > 255) {
                                    while (patternCount > 255) {
                                        entry.Text += patternID + ",255,";
								        entry.Length += 2;  
                                        patternCount -= 255;                                   
                                    }
                                    entry.Text += patternID+ ","+patternCount + ",";
								    entry.Length += 2;                                    
                                } else {
                                    entry.Text += patternID+ ","+patternCount + ",";
								    entry.Length += 2;
                                }
							} else {
								entry.Text += thisPattern.Cells[0].TileID + ",";
								entry.Length++;
							}
						}
						break;
						
					case Map.ReductionType.None:
					default:
						//write out the entire contents of the map cells to the file
						foreach (MapCell thisCell in addMap.Cells) {
							entry.Text += thisCell.TileID + ",";
							entry.Length++;
						}
						break;
						
				}
                entry.Text += Environment.NewLine; 
                this.entries.Add(entry);
        }

        public void Write(string fileName) {
            using (StreamWriter writer = new StreamWriter(fileName,false)) {
                writer.WriteLine("#ifndef MAP_DATA_H");
                writer.WriteLine("#define MAP_DATA_H");

                writer.WriteLine("const unsigned short PROGMEM MAP_DATA_INDEX[] = {");
                for (int i=0; i < this.entries.Count; i++) {
                    writer.WriteLine(this.entries[i].Length + ",");
                }
                writer.WriteLine("};");
                writer.WriteLine();
                writer.WriteLine("const unsigned char PROGMEM MAP_DATA[] = {");
                foreach (MapDataEntry thisEntry in this.entries) {
                    writer.Write(thisEntry.Text);
                    writer.WriteLine();
                }
                writer.WriteLine("};");
                writer.WriteLine();

                writer.WriteLine("#endif");
            }
        }
    }
}