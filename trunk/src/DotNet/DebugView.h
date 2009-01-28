#pragma once


/// This class provides a list like view in the debugger for
/// classes that implement ICollection.
generic<typename TViewedContainer, typename TViewedItems>
where TViewedContainer : System::Collections::Generic::IEnumerable<TViewedItems>
[System::Diagnostics::DebuggerDisplay("")]
private ref class DebugView
{
public:
  DebugView(TViewedContainer viewedContainer)
    :viewedContainer_(viewedContainer)
  {
  }

  [System::Diagnostics::DebuggerBrowsable(System::Diagnostics::DebuggerBrowsableState::RootHidden)]
  property array<TViewedItems>^ Keys
  {
    array<TViewedItems>^ get()
    {
      typedef System::Collections::Generic::List<TViewedItems> TList;
      TList^ outList = gcnew TList();

      for each(TViewedItems key in viewedContainer_)
      {
        outList->Add(key);
      }
      return outList->ToArray();
    }
  }

private:
  TViewedContainer viewedContainer_;
};
