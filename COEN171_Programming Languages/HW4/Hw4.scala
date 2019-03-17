object Hw4{
	def main(args: Array[String]){
		val json_string = scala.io.Source.fromFile("38.json").getLines.mkString
		val listOfHash: scala.collection.mutable.MutableList[String] = scala.collection.mutable.MutableList()
		
		var bool: Boolean = false
		var str: String = ""
		for(i <- 0 until json_string.length) {
			var char = json_string(i)
			if(char == '#')
				bool = true;
			else {
				if(bool) {
					if('A'.max(char) == 'Z'.min(char) || 'a'.max(char) == 'z'.min(char) || '0'.max(char) == '9'.min(char))
						str = str+char
					 else {
						if(str.length > 0 && (str.compareTo("Profile") > 0 || str.compareTo("Profile") < 0)) {
							//print("#"+str+" ")
							listOfHash += str
						}
						str=""
						bool = false
					}
				}
			}
		}
		//println()
		println("Total distinct hash tags: "+listOfHash.distinct.length)
		println("File contains total hash tags: "+listOfHash.length)
		println("Most popular hashtag: " +listOfHash.groupBy(identity).mapValues(_.size).maxBy(_._2)._1)
	}
}

