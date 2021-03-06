// FRAGMENT(include)
#include <iostream>
#include <seqan/find.h>
#include <seqan/seq_io.h>
#include <seqan/journaled_set.h>

using namespace seqan;

// FRAGMENT(searchAtBorder)
template <typename TJournalEntriesIterator, typename TJournal, typename TPattern>
void _searchAtBorder(String<int> & hitTarget,
                    TJournalEntriesIterator & entriesIt,
                    TJournal const & journal,
                    TPattern const & needle)
{
    typedef typename Position<TJournal>::Type TPosition;

    // Define region before the border to the next node to search for the pattern.
    TPosition infixBegin = entriesIt->virtualPosition + _max(0,(int)entriesIt->length - (int)length(needle) + 1);
    TPosition infixEnd = _min(length(journal),entriesIt->virtualPosition + entriesIt->length + length(needle) - 1);

    TPattern tmpInsBuffer = infix(journal, infixBegin, infixEnd);
    Finder<TPattern const> finder(tmpInsBuffer);
    Pattern<TPattern, Horspool> pattern(needle);
    while (find(finder, pattern))
    {
        appendValue(hitTarget, infixBegin + beginPosition(finder));
    }
}

// FRAGMENT(findInPatchNodePart1)
template <typename TJournalEntriesIterator, typename TJournal, typename TPattern>
void _findInPatchNode(String<int> & hitTarget,
                      TJournalEntriesIterator & entriesIt,
                      TJournal const & journal,
                      TPattern const & needle)
{
    typedef typename Position<TJournal>::Type TPosition;

    TPosition infixBegin = entriesIt->virtualPosition;
    TPosition infixEnd = entriesIt->virtualPosition + entriesIt->length;

    TPattern tmpInsBuffer = infix(journal, infixBegin, infixEnd);
    Finder<TPattern const> finder(tmpInsBuffer);
    Pattern<TPattern, Horspool> pattern(needle);

    while (find(finder, pattern))
        appendValue(hitTarget, entriesIt->virtualPosition + beginPosition(finder));
}

// FRAGMENT(findInOriginalNode)
template <typename TJournalEntriesIterator, typename TPattern>
void _findInOriginalNode(String<int> & hitTarget,
                         TJournalEntriesIterator & entriesIt,
                         TPattern const & pattern,
                         String<int> const & refHits)
{
    // Define an Iterator which iterates over the reference hit set.
    typedef typename Iterator<String<int> const, Standard>::Type THitIterator;

    // Check if hits exist in the reference.
    if(!empty(refHits))
    {
        // Find upper bound to physical position in sorted refHits.
        THitIterator itHit = std::upper_bound(begin(refHits),end(refHits),(int)entriesIt->physicalPosition);
        // Make sure we do not miss hits that begin at physical position of current node.
        if(itHit != begin(refHits) && *(itHit - 1) >= (int)entriesIt->physicalPosition)
            --itHit;
        // Store all hits that are found in the region of the reference which is covered by this node.
        while((int)*itHit < ((int)entriesIt->physicalPosition + (int)entriesIt->length - (int)length(pattern) + 1) && itHit != end(refHits))
        {
            appendValue(hitTarget, entriesIt->virtualPosition + (*itHit - (int)entriesIt->physicalPosition));
            ++itHit;
        }
    }
}

// FRAGMENT(findPatternInJournalString)
template <typename TValue, typename THostSpec, typename TJournalSpec, typename TBufferSpec, typename TPattern>
void findPatternInJournalString(String<int> & hitTarget,
                           String<TValue, Journaled<THostSpec, TJournalSpec, TBufferSpec> > const & journal,
                           TPattern const & pattern,
                           String<int> const & refHits)
{
    typedef String<TValue, Journaled<THostSpec, TJournalSpec, TBufferSpec> > const TJournal;
    typedef typename JournalType<TJournal>::Type TJournalEntries;
    typedef typename Iterator<TJournalEntries>::Type TJournalEntriesIterator;

    if (length(pattern) > length(journal))
        return;

    TJournalEntriesIterator it = begin(journal._journalEntries);
    TJournalEntriesIterator itEnd = findInJournalEntries(journal._journalEntries, length(journal) - length(pattern) + 1) + 1;

    while(it != itEnd)
    {
        if (it->segmentSource == SOURCE_ORIGINAL)
        {   // Find a possible hit in the current source vertex.
            _findInOriginalNode(hitTarget, it, pattern, refHits);
        }
        if (it->segmentSource == SOURCE_PATCH)
        {  // Search for pattern within the patch node.
            _findInPatchNode(hitTarget, it, journal, pattern);
        }
        // Scan the border for a possible match.
        _searchAtBorder(hitTarget, it, journal, pattern);
        ++it;
    }
}

// FRAGMENT(findPatternInReference)
template <typename TString>
void findPatternInReference(String<int> & hits,
                            TString const & reference,
                            TString const & needle)
{
    // Check whether the pattern fits into the sequence.
    if (length(needle) > length(reference))
        return;

    Finder<TString const> finder(reference);
    Pattern<TString, Horspool> pattern(needle);
    while (find(finder, pattern))
        appendValue(hits, beginPosition(finder));
}

// FRAGMENT(searchPatternPart1)
template <typename TString, typename TPattern>
void searchPattern(StringSet<String<int> > & hitSet,
                   StringSet<TString, Owner<JournaledSet> > const & journalSet,
                   TPattern const & pattern)
{
    typedef typename Host<TString>::Type THost;

    // Check for valid initial state.
    if (empty(globalReference(journalSet)))
    {
        std::cout << "No reference set. Aborted search!" << std::endl;
        return;
    }

    // Reset the hitSet to avoid phantom hits coming from a previous search.
    clear(hitSet);
    resize(hitSet, length(journalSet) + 1);
    // Access the reference sequence.
    THost & globalRef = globalReference(journalSet);
    // Search for pattern in the reference sequence.
    findPatternInReference(hitSet[0], globalRef, pattern);

    // Search for pattern in the journaled sequences.
    for (unsigned i = 0; i < length(journalSet); ++i)
        findPatternInJournalString(hitSet[i+1], journalSet[i], pattern, hitSet[0]);
}

// FRAGMENT(laodAndJoin)
template <typename TString, typename TStream, typename TSpec>
inline int
loadAndJoin(StringSet<TString, Owner<JournaledSet> > & journalSet,
            TStream & stream,
            JoinConfig<TSpec> const & joinConfig)
{
    typedef typename Host<TString>::Type THost;

    RecordReader<std::ifstream, SinglePass<> > reader(stream);

    clear(journalSet);

    String<char> seqId;
    THost sequence;

    // No sequences in the fasta file!
    if (atEnd(reader))
    {
        std::cerr << "Empty FASTA file." << std::endl;
        return -1;
    }
    // First read sequence for reference sequence.
    if (readRecord(seqId, sequence, reader, Fasta()) != 0)
    {
        std::cerr << "ERROR reading FASTA." << std::endl;
        return 1;
    }
    // We have to create the global reference sequence otherwise we loose the information after this function terminates.
    createGlobalReference(journalSet, sequence);

    // If there are more
    while (!atEnd(reader))
    {
        if (readRecord(seqId, sequence, reader, Fasta()) != 0)
        {
            std::cerr << "ERROR reading FASTA." << std::endl;
            return 1;
        }
        appendValue(journalSet, TString(sequence));
        join(journalSet, length(journalSet) - 1, joinConfig);
    }
    return 0;
}

// FRAGMENT(main)
int main()
{
    // Definition of the used types.
    typedef String<Dna,Alloc<> > TSequence;
    typedef String<Dna,Journaled<Alloc<>,SortedArray,Alloc<> > > TJournal;
    typedef StringSet< TJournal, Owner<JournaledSet> > TJournaledSet;

    // Open the stream to the file containing the sequences.
    String<char> seqDatabasePath = "/Users/rahn_r/Downloads/sequences.fasta";
//    String<char> seqDatabasePath =  "/path/to/your/fasta/file/sequences.fasta";
    std::ifstream databaseFile(toCString(seqDatabasePath), std::ios_base::in);
    if(!databaseFile.good())
    {
        std::cerr << "Cannot open file <" << seqDatabasePath << ">!" << std::endl;
    }


    // Reading each sequence and journal them.
    TJournaledSet journalSet;
    JoinConfig<GlobalAlign<JournaledCompact> > joinConfig;
    loadAndJoin(journalSet, databaseFile, joinConfig);
    databaseFile.close();

    // Define a pattern and start search.
    StringSet<String<int> > hitSet;
    TSequence pattern = "GTGGT";
    std::cout << "Search for: " << pattern << "\n";
    searchPattern(hitSet, journalSet, pattern);


    if (empty(hitSet[0]))
    {
        std::cout << "No hit in reference " << std::endl;
    }
    else
    {
        std::cout << "Hit in reference " << " at ";
        for (unsigned j = 0; j < length(hitSet[0]); ++j)
            std::cout << hitSet[0][j] << ": " << infix(globalReference(journalSet), hitSet[0][j],hitSet[0][j] + length(pattern)) << "\t";
    }
    std::cout << std::endl;

    for (unsigned i = 1; i < length(hitSet); ++i)
    {
        if (empty(hitSet[i]))
        {
            std::cout << "No hit in sequence " << i - 1 << std::endl;
        }
        else
        {
            std::cout << "Hit in sequence " << i - 1 << " at ";
            for (unsigned j = 0; j < length(hitSet[i]); ++j)
                std::cout << hitSet[i][j] << ": " << infix(value(journalSet,i-1), hitSet[i][j],hitSet[i][j] + length(pattern)) << "\t";
        }
        std::cout << std::endl;
    }
    return 0;
}
