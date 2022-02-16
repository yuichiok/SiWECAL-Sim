#include "ShapedConversionProcessor.h"
#include <iostream>

#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>

// ----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"

#ifdef MARLIN_USE_AIDA
#include <marlin/AIDAProcessor.h>
#include <AIDA/IHistogramFactory.h>
#include <AIDA/ICloud1D.h>
//#include <AIDA/IHistogram1D.h>
#endif // MARLIN_USE_AIDA


using namespace lcio ;
using namespace marlin ;


ShapedConversionProcessor aShapedConversionProcessor ;


ShapedConversionProcessor::ShapedConversionProcessor() : Processor("ShapedConversionProcessor") {

    // modify processor description
    _description = "ShapedConversionProcessor does whatever it does ..." ;


    // register steering parameters: name, description, class-variable, default value
    registerInputCollection( LCIO::MCPARTICLE,
            "CollectionName" , 
            "Name of the MCParticle collection"  ,
            _colName ,
            std::string("MCParticle")
    );
}



void ShapedConversionProcessor::init() { 

    streamlog_out(DEBUG) << "   init called  " << std::endl ;

    // usually a good idea to
    printParameters() ;

    _nRun = 0 ;
    _nEvt = 0 ;

}


void ShapedConversionProcessor::processRunHeader( LCRunHeader* run) { 

    _nRun++ ;
} 



void ShapedConversionProcessor::processEvent( LCEvent * evt ) { 


    // this gets called for every event 
    // usually the working horse ...


#ifdef MARLIN_USE_AIDA

    // define a histogram pointer
    static AIDA::ICloud1D* hMCPEnergy ;    

    if( isFirstEvent() ) { 

        hMCPEnergy = AIDAProcessor::histogramFactory(this)->
            createCloud1D( "hMCPEnergy", "energy of the MCParticles", 100 ) ; 

    }
#endif // MARLIN_USE_AIDA

    // try to get lcio collection (exits if collection is not available)
    // NOTE: if the AIDAProcessor is activated in your steering file and Marlin is linked with
    //      RAIDA you may get the message: "*** Break *** segmentation violation" followed by a
    //      stack-trace (generated by ROOT) in case the collection is unavailable. This happens
    //      because ROOT is somehow catching the exit signal commonly used to exit a program
    //      intentionally. Sorry if this messsage may confuse you. Please ignore it!
    LCCollection* col = evt->getCollection( _colName ) ;


    // Alternativelly if you do not want Marlin to exit in case of a non-existing collection
    // use the following (commented out) code:
    //LCCollection* col = NULL;
    //try{
    //    col = evt->getCollection( _colName );
    //}
    //catch( lcio::DataNotAvailableException e )
    //{
    //    streamlog_out(WARNING) << _colName << " collection not available" << std::endl;
    //    col = NULL;
    //}

    // this will only be entered if the collection is available
    if( col != NULL ){

        int nMCP = col->getNumberOfElements()  ;

        for(int i=0; i< nMCP ; i++){

            MCParticle* p = dynamic_cast<MCParticle*>( col->getElementAt( i ) ) ;

#ifdef MARLIN_USE_AIDA
            // fill histogram from LCIO data :
            hMCPEnergy->fill( p->getEnergy() ) ;
#endif
        } 
    }



    //-- note: this will not be printed if compiled w/o MARLINDEBUG=1 !

    streamlog_out(DEBUG) << "   processing event: " << evt->getEventNumber() 
        << "   in run:  " << evt->getRunNumber() << std::endl ;



    _nEvt ++ ;
}



void ShapedConversionProcessor::check( LCEvent * evt ) { 
    // nothing to check here - could be used to fill checkplots in reconstruction processor
}


void ShapedConversionProcessor::end(){ 

    //   std::cout << "ShapedConversionProcessor::end()  " << name() 
    // 	    << " processed " << _nEvt << " events in " << _nRun << " runs "
    // 	    << std::endl ;

}

