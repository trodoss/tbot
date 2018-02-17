using System;
using System.IO;

namespace map_pack
{
    class Program
    {
        static void Main(string[] args)
        {
            			
			
            MapDataWriter mapWriter = new MapDataWriter();
            ElementDataWriter elementWriter = new ElementDataWriter();
			
			TMXFile tmxFile = TMXFileReader.Load("lvl_1_1.tmx");
			
			Map backdropMap = tmxFile.ToMap("backdrop");
            backdropMap = RLEReducer.Reduce(backdropMap);
            backdropMap.Reduction = Map.ReductionType.RLE;	
            mapWriter.AddMap(backdropMap);

            ElementList elementList = tmxFile.ToElementList("elements");
            elementWriter.AddElementList(elementList);
            
			elementWriter.Write("element_data.h");
            elementWriter = null; 
            
            mapWriter.Write("map_data.h");
            mapWriter = null;
        }
    }
}
