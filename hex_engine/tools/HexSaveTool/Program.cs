using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.Json;

namespace HexSaveTool;

// This tiny utility will eventually help us inspect and validate world save
// files without dragging the native engine into editor-only tasks.
internal static class Program
{
    private static int Main(string[] args)
    {
        if (args.Length == 0)
        {
            PrintUsage();
            return 1;
        }

        // We keep the first version deliberately narrow: a single JSON file path
        // is enough to prove the toolchain and the data contract.
        string inputPath = args[0];
        if (!File.Exists(inputPath))
        {
            Console.Error.WriteLine($"File not found: {inputPath}");
            return 2;
        }

        string jsonText = File.ReadAllText(inputPath);
        WorldDocument? document = JsonSerializer.Deserialize<WorldDocument>(jsonText, JsonOptions());
        if (document is null)
        {
            Console.Error.WriteLine("Could not parse the document.");
            return 3;
        }

        PrintReport(inputPath, document);
        return 0;
    }

    private static void PrintUsage()
    {
        Console.WriteLine("HexSaveTool");
        Console.WriteLine("Usage: HexSaveTool <world.json>");
        Console.WriteLine("Expected JSON shape:");
        Console.WriteLine("{");
        Console.WriteLine("  \"version\": 1,");
        Console.WriteLine("  \"seed\": 123,");
        Console.WriteLine("  \"cells\": [");
        Console.WriteLine("    { \"q\": 0, \"r\": 0, \"kind\": \"Mover\", \"age\": 0 }");
        Console.WriteLine("  ]");
        Console.WriteLine("}");
    }

    private static void PrintReport(string inputPath, WorldDocument document)
    {
        // A stable summary keeps this tool useful even before we add editing or
        // export features.
        Console.WriteLine($"File: {inputPath}");
        Console.WriteLine($"Version: {document.Version}");
        Console.WriteLine($"Seed: {document.Seed}");
        Console.WriteLine($"Cell count: {document.Cells.Count}");

        IEnumerable<string> kinds = document.Cells.Select(cell => cell.Kind);
        foreach (IGrouping<string, string> group in kinds.GroupBy(kind => kind).OrderBy(group => group.Key))
        {
            Console.WriteLine($"  {group.Key}: {group.Count()}");
        }
    }

    private static JsonSerializerOptions JsonOptions()
    {
        // The options are isolated in one place so the save format can evolve in
        // a controlled way later.
        return new JsonSerializerOptions
        {
            PropertyNameCaseInsensitive = true,
            ReadCommentHandling = JsonCommentHandling.Skip,
        };
    }
}

// The document types stay tiny on purpose. We want a stable contract that can
// be extended without breaking the first save format.
internal sealed record WorldDocument(
    int Version,
    ulong Seed,
    List<CellDocument> Cells
);

// Each serialized cell mirrors the data we will keep in the native world.
internal sealed record CellDocument(
    int Q,
    int R,
    string Kind,
    uint Age
);

