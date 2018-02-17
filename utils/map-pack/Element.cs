using System;
using System.Collections;
using System.Collections.Generic;

namespace map_pack {
    public class Element {
        public string Type {get; set;}
        public string State {get; set;}
        public int X {get; set;}
        public int Y {get; set;}
    }
    
    public class ElementList : List<Element> { }
}