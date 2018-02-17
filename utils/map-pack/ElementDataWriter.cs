using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;

namespace map_pack {
    public class ElementDataWriter {
        private List<ElementList> entries;

        public ElementDataWriter() {
            this.entries = new List<ElementList>();
        }

        public void AddElementList(ElementList list) {
            this.entries.Add(list);
        }

        public void Write(string fileName) {
            using (StreamWriter writer = new StreamWriter(fileName,false)) {
                writer.WriteLine("#ifndef ELEMENT_DATA_H");
                writer.WriteLine("#define ELEMENT_DATA_H");

                writer.WriteLine("const unsigned short PROGMEM ELEMENT_DATA_INDEX[] = {");
                foreach (ElementList thisList in this.entries) {
                    writer.WriteLine(thisList.Count + ",");
                }
                writer.WriteLine("};");
                writer.WriteLine();


                writer.WriteLine("const unsigned char PROGMEM ELEMENT_DATA[] = {");
                foreach (ElementList thisList in this.entries) {
                    foreach(Element thisElement in thisList) {
                        writer.WriteLine(thisElement.Type + "," + thisElement.X + "," + thisElement.Y + "," + thisElement.State + ",");
                    }
                    writer.WriteLine();         
                }
                writer.WriteLine("};");
                writer.WriteLine();

                writer.WriteLine("#endif");
            }            
        }
    }
}