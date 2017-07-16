struct treeNode
{
    string val ;
    bool isLeaf ;
    unordered_map<char, treeNode*> children ;
    treeNode(string v): val(v), isLeaf(false) {}
} ;


 

class AutocompleteSystem {
public:

    
    AutocompleteSystem(vector<string> sentences, vector<int> times) 
    {
        sentenceTimes.clear() ;
            
        root = new treeNode("") ;
        
        for(int i=0; i<sentences.size(); i++)  
            dataStorer( sentences[i], times[i] ) ;
        
        curSentence = "" ;      
    }
    
    vector<string> input(char c) 
    {
        if( c == '#' )
        {
            dataStorer( curSentence, 1 ) ;
            curSentence = "" ;
            return {} ;
        }    
        
        curSentence.push_back( c ) ;
        vector<string> possibleStr = Searcher( curSentence ) ;
                
        return top3Finder( possibleStr ) ;
    }
    
    void dataStorer(string sentence, int time)
    {
        sentenceTimes[ sentence ] += time ;
        
        treeNode* curNode = root ;
        for(int i=0; i<sentence.length(); i++)
        {
            if( !curNode->children.count( sentence[i] ) )
                 curNode->children[ sentence[i] ] = new treeNode( sentence.substr(0, i+1) ) ;
            curNode = curNode->children[ sentence[i] ] ;
        }  
        curNode->isLeaf = true ;
    }    
    
    vector<string> Searcher(string sentence)
    {
        treeNode* curNode = root ;
        vector<string> leaves ;
        
        for(int i=0; i<sentence.length(); i++)  
        {
            if( !curNode->children.count( sentence[i] ) )
                return {} ;
            curNode = curNode->children[ sentence[i] ] ;
        }
        
        allChildren( curNode, leaves ) ;
        return leaves ;
    }    
    
    
    void allChildren(treeNode* node, vector<string>& leaves)
    {
        if( node->isLeaf)
            leaves.push_back( node->val ) ;

        for(auto it=node->children.begin(); it!=node->children.end(); ++it)
                allChildren( it->second, leaves ) ;              
    }    
    
    vector<string> top3Finder(vector<string>& possible)
    {
        string curStr1 = "", curStr2 = "", curStr3 = "";

        for(string str : possible)
        {
            if( comparator( str, curStr1 ) )
            {          
                curStr3 = curStr2 ;
                curStr2 = curStr1 ;
                curStr1 = str ; 
            }
            else if( comparator( str, curStr2 ) )
            {    
                curStr3 = curStr2 ;
                curStr2 = str ;
            }
            else if( comparator( str, curStr3 ) )
                curStr3 = str ;        
        }                
            
        vector<string> Res = {curStr1, curStr2, curStr3} ;
        while( !Res.empty() && Res.back() == "" ) Res.pop_back() ;
        
        return Res ;
    }    
        
            
    
    bool comparator(string A, string B)
    {
        if( sentenceTimes[A] > sentenceTimes[B] )
            return true ;
        else if( sentenceTimes[A] < sentenceTimes[B] )
            return false ;
        else
        {
            int minLen = min( A.length(), B.length() ) ;
            for(int i=0; i<minLen; i++)
            { 
                if( A[i]-'0' < B[i]-'0' )
                    return true ;
                
                else if( A[i]-'0' > B[i]-'0' )
                    return false ;
            }    
            return A.length() < B.length() ;
        }
    }    
    

private:
    unordered_map<string,int> sentenceTimes ;
    treeNode* root ;
    string curSentence ;
};

/**
 * Your AutocompleteSystem object will be instantiated and called as such:
 * AutocompleteSystem obj = new AutocompleteSystem(sentences, times);
 * vector<string> param_1 = obj.input(c);
 */
